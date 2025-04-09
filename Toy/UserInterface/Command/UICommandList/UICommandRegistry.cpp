#include "pch.h"
#include "UICommandRegistry.h"
#include "Utility.h"
#include "../../UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../../UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../../UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../../UIComponent/Components/TextArea.h"
#include "../Include/IRenderer.h"

AttachComponentCommand::AttachComponentCommand(UIComponent* parent,
	unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept :
	UICommand{ nullptr },
	m_parent{ parent },
	m_attach{ move(component) },
	m_pos{ relativePos },
	m_detach{ nullptr } 
{}

bool AttachComponentCommand::Execute()
{
	m_detach = m_attach.get();
	m_failureResult = UIEx(m_parent).AttachComponent(move(m_attach), m_pos);
	return m_failureResult == nullptr;
}

bool AttachComponentCommand::Undo()
{
	if (auto [detach, parent] = UIEx(m_detach).DetachComponent(); detach)
	{
		m_attach = move(detach);
		return true;
	}
	return false;
}

bool AttachComponentCommand::Redo()
{
	m_failureResult = UIEx(m_parent).AttachComponent(move(m_attach), m_pos);
	return m_failureResult == nullptr;
}

unique_ptr<UIComponent> AttachComponentCommand::GetFailureResult() noexcept
{
	return move(m_failureResult);
}

//////////////////////////////////////////////////////////////////

DetachComponentCommand::DetachComponentCommand(UIComponent* detach) noexcept :
	UICommand{ nullptr },
	m_detach{ detach },
	m_component{ nullptr },
	m_parent{ nullptr },
	m_result{}
{}

bool DetachComponentCommand::Execute()
{
	XMINT2 pos = m_detach->GetRelativePosition();
	auto [component, parent] = UIEx(m_detach).DetachComponent();
	if (!component) return false;

	m_position = pos;
	m_component = move(component); //원본은 저장하고 클론은 밖으로 
	m_parent = parent;

	m_result = { m_component->Clone(), parent };

	return true;
}

bool DetachComponentCommand::Undo()
{
	UIComponent* detach = m_component.get();
	auto resultComponent = UIEx(m_parent).AttachComponent(move(m_component), m_position);
	if (resultComponent) return false;

	m_detach = detach;
	return true;
}

bool DetachComponentCommand::Redo()
{
	auto [component, parent] = UIEx(m_detach).DetachComponent();
	if (!component) return false;

	m_component = move(component);
	if(!CompareUniquePtr(m_result.first, m_component)) //Undo, Redo를 계속하면 m_result에 값이 있어서 소멸자가 호출된다. 그래서 texture의 reference count가 꼬이게 된다.
		m_result = { m_component->Clone(), parent };

	return m_result.first != nullptr;
}

pair<unique_ptr<UIComponent>, UIComponent*> DetachComponentCommand::GetResult() noexcept
{
	auto& [detach, parent] = m_result;
	return { move(detach), parent };
}

//////////////////////////////////////////////////////////////////

SetRelativePositionCommand::SetRelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept :
	UICommand{ component }, m_record{ relativePos }
{}

bool SetRelativePositionCommand::Execute()
{
	const auto& prevPos = GetTarget()->GetRelativePosition();
	m_record.previous = prevPos;
	ReturnIfFalse(GetTarget()->SetRelativePosition(m_record.current));

	return true;
}

bool SetRelativePositionCommand::Undo() { return GetTarget()->SetRelativePosition(m_record.previous); }
bool SetRelativePositionCommand::Redo() { return GetTarget()->SetRelativePosition(m_record.current); }

void SetRelativePositionCommand::PostMerge(unique_ptr<UICommand> other) noexcept
{
	auto otherCmd = static_cast<SetRelativePositionCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}

//////////////////////////////////////////////////////////////////

SetSizeCommand::SetSizeCommand(UIComponent* component, const XMUINT2& size) noexcept :
	UICommand{ component }, m_record{ size }
{}

bool SetSizeCommand::Execute()
{
	m_record.previous = GetTarget()->GetSize();
	return GetTarget()->ChangeSize(m_record.current);
}

bool SetSizeCommand::Undo() { return GetTarget()->ChangeSize(m_record.previous); }
bool SetSizeCommand::Redo() { return GetTarget()->ChangeSize(m_record.current); }

void SetSizeCommand::PostMerge(unique_ptr<UICommand> other) noexcept
{
	auto otherCmd = static_cast<SetSizeCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}

//////////////////////////////////////////////////////////////////

RenameRegionCommand::RenameRegionCommand(UIComponent* component, const string& region) noexcept :
	UICommand{ component }, m_record{ region }
{}

bool RenameRegionCommand::Execute()
{
	m_record.previous = GetTarget()->GetRegion();
	GetTarget()->RenameRegion(m_record.current);
	return true;
}

bool RenameRegionCommand::Undo() { GetTarget()->RenameRegion(m_record.previous); return true; }
bool RenameRegionCommand::Redo() { GetTarget()->RenameRegion(m_record.current); return true; }

//////////////////////////////////////////////////////////////////

RenameCommand::RenameCommand(UIComponent* component, const string& name) noexcept :
	UICommand{ component }, m_record{ name }
{}

bool RenameCommand::Execute()
{
	m_record.previous = GetTarget()->GetName();
	return GetTarget()->Rename(m_record.current);
}

bool RenameCommand::Undo() { return GetTarget()->Rename(m_record.previous); }
bool RenameCommand::Redo() { return GetTarget()->Rename(m_record.current); }

//////////////////////////////////////////////////////////////////

//namespace {
//	//variant안에 값들은 UIComponent에서 상속 받은 것이라서 visit로 값을 하나 빼와서 캐스팅 한다.
//	//get<UIComponent*>는 되지 않았다. 컴파일 타임에는 모르기 때문에 ImageGrid1* 이렇게 넣어야만 가져올 수 있다.
//	template<typename Child>
//	UIComponent* ExtractUIComponent(const Child& imgGridVariant) noexcept {
//		return std::visit([](auto* imgGrid) -> UIComponent* { return imgGrid; }, imgGridVariant);
//	}
//}

//SetSourceCommand::SetSourceCommand(ImageGrid1* imageGrid1, const Rectangle& source) noexcept :
//	Command{ imageGrid1 },
//	m_imgGrid1{ imageGrid1 },
//	m_record{ source }
//{}
//
//bool SetSourceCommand::Execute()
//{
//	m_record.previous = m_imgGrid1->GetSource();
//	m_imgGrid1->SetSource(m_record.current);
//	return true;
//}
//
//bool SetSourceCommand::Undo() { m_imgGrid1->SetSource(m_record.previous); return true; }
//bool SetSourceCommand::Redo() { m_imgGrid1->SetSource(m_record.current); return true; }
//
//void SetSourceCommand::PostMerge(unique_ptr<Command> other) noexcept
//{
//	auto otherCmd = static_cast<SetSourceCommand*>(other.get());
//	m_record.current = otherCmd->m_record.current;
//}

//////////////////////////////////////////////////////////////////	

//SetSource39Command::SetSource39Command(const ImageGrid39Variant& imgGridVariant, const vector<Rectangle>& sources) noexcept :
//	Command{ ExtractUIComponent(imgGridVariant) },
//	m_imgGridVariant{ imgGridVariant },
//	m_record{ sources }
//{}
//
//optional<vector<Rectangle>> SetSource39Command::GetSources() noexcept
//{
//	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
//		return (*imgGrid3)->GetSources();
//
//	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
//		return (*imgGrid9)->GetSources();
//
//	return nullopt;
//}
//
//bool SetSource39Command::SetSources(const vector<Rectangle>& sources) noexcept
//{
//	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
//		return (*imgGrid3)->SetSources(sources);
//
//	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
//		return (*imgGrid9)->SetSources(sources);
//
//	return false;
//}
//
//bool SetSource39Command::Execute()
//{
//	auto sources = GetSources();
//	ReturnIfFalse(sources);
//
//	m_record.previous = *sources;
//	return SetSources(m_record.current);
//}
//
//bool SetSource39Command::Undo() { return SetSources(m_record.previous); }
//bool SetSource39Command::Redo() { return SetSources(m_record.current); }
//
//void SetSource39Command::PostMerge(unique_ptr<Command> other) noexcept
//{
//	auto otherCmd = static_cast<SetSource39Command*>(other.get());
//	m_record.current = otherCmd->m_record.current;
//}

//////////////////////////////////////////////////////////////////	

//SetFilenameCommand::SetFilenameCommand(const ImageGridVariant& imgGridVariant, 
//	IRenderer* renderer, const wstring& filename) noexcept :
//	Command{ ExtractUIComponent(imgGridVariant) },
//	m_imgGridVariant{ imgGridVariant },
//	m_renderer{ renderer },
//	m_record{ filename }
//{}
//
//wstring SetFilenameCommand::GetFilename() const noexcept
//{
//	return visit([](auto* imgGrid) -> wstring { 
//		return imgGrid ? imgGrid->GetFilename() : L"";
//		}, m_imgGridVariant);
//}
//
//bool SetFilenameCommand::SetFilename(const wstring& filename)
//{
//	return visit([this, filename](auto* imgGrid) -> bool {
//		if (imgGrid)
//		{
//			//imgGrid->SetFilename(filename);
//			//return m_renderer->LoadComponent(imgGrid);
//		}
//		return false;
//		}, m_imgGridVariant);
//}
//
//bool SetFilenameCommand::Execute()
//{
//	m_record.previous = GetFilename();
//	return SetFilename(m_record.current);
//}
//
//bool SetFilenameCommand::Undo() { return SetFilename(m_record.previous); }
//bool SetFilenameCommand::Redo() { return SetFilename(m_record.current); }

//////////////////////////////////////////////////////////////////

//SetSourceAndDividerCommand::SetSourceAndDividerCommand(const ImageGrid39Variant& imgGridVariant,
//	const SourceDivider& srcDivider) noexcept :
//	Command{ ExtractUIComponent(imgGridVariant) },
//	m_imgGridVariant{ imgGridVariant },
//	m_record{ srcDivider }
//{}
//
//SourceDivider SetSourceAndDividerCommand::GetSourceAndDivider() const noexcept
//{
//	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
//		return (*imgGrid3)->GetSourceAnd2Divider();
//
//	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
//		return (*imgGrid9)->GetSourceAnd4Divider();
//
//	return {};
//}
//
//bool SetSourceAndDividerCommand::SetSourceAndDivider(const SourceDivider& srcDivider) noexcept
//{
//	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
//		return (*imgGrid3)->SetSourceAnd2Divider(srcDivider);
//
//	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
//		return (*imgGrid9)->SetSourceAnd4Divider(srcDivider);
//
//	return false;
//}
//
//bool SetSourceAndDividerCommand::Execute()
//{
//	auto srcDivider = GetSourceAndDivider();
//	ReturnIfFalse(!srcDivider.Empty());
//
//	m_record.previous = srcDivider;
//	return SetSourceAndDivider(m_record.current);
//}
//
//bool SetSourceAndDividerCommand::Undo() { return SetSourceAndDivider(m_record.previous); }
//bool SetSourceAndDividerCommand::Redo() { return SetSourceAndDivider(m_record.current); }
//
//void SetSourceAndDividerCommand::PostMerge(unique_ptr<Command> other) noexcept
//{
//	auto otherCmd = static_cast<SetSourceAndDividerCommand*>(other.get());
//	m_record.current = otherCmd->m_record.current;
//}

//////////////////////////////////////////////////////////////////

SetTextCommand::SetTextCommand(TextArea* textArea, const wstring& text) noexcept :
	UICommand{ textArea },
	m_textArea{ textArea },
	m_record{ text }
{}

bool SetTextCommand::Execute()
{
	m_record.previous = m_textArea->GetText();
	return m_textArea->SetText(m_record.current);
}

bool SetTextCommand::Undo() { return m_textArea->SetText(m_record.previous); }
bool SetTextCommand::Redo() { return m_textArea->SetText(m_record.current); }