#include "pch.h"
#include "CommandRegistry.h"
#include "../../Utility.h"
#include "../UIComponentEx.h"
#include "../Component/ImageGrid1.h"
#include "../Component/ImageGrid3.h"
#include "../Component/ImageGrid9.h"
#include "../Include/IRenderer.h"

using namespace UIComponentEx;

AttachComponentCommand::AttachComponentCommand(UIComponent* addable,
	unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept :
	Command{ nullptr }, 
	m_addable{ addable },
	m_attach{ move(component) },
	m_pos{ relativePos },
	m_detach{ nullptr } 
{}

bool AttachComponentCommand::Execute()
{
	m_detach = m_attach.get();
	m_failureResult = m_addable->AttachComponent(move(m_attach), m_pos);
	return m_failureResult == nullptr;
}

bool AttachComponentCommand::Undo()
{
	if (auto [detach, parent] = m_detach->DetachComponent(); detach)
	{
		m_attach = move(detach);
		return true;
	}
	return false;
}

bool AttachComponentCommand::Redo()
{
	m_failureResult = m_addable->AttachComponent(move(m_attach), m_pos);
	return m_failureResult == nullptr;
}

unique_ptr<UIComponent> AttachComponentCommand::GetFailureResult() noexcept
{
	return move(m_failureResult);
}

//////////////////////////////////////////////////////////////////

DetachComponentCommand::DetachComponentCommand(UIComponent* detach) noexcept :
	Command{ nullptr },
	m_detach{ detach },
	m_component{ nullptr },
	m_addable{ nullptr },
	m_result{}
{}

bool DetachComponentCommand::Execute()
{
	auto pos = GetRelativePosition(m_detach);
	if (!pos.has_value()) return false;

	auto [component, addable] = m_detach->DetachComponent();
	if (!component) return false;

	m_position = *pos;
	m_component = move(component); //������ �����ϰ� Ŭ���� ������ 
	m_addable = addable;

	m_result = { m_component->Clone(), addable };

	return true;
}
bool DetachComponentCommand::Undo()
{
	UIComponent* detach = m_component.get();
	auto resultComponent = m_addable->AttachComponent(move(m_component), m_position);
	if (resultComponent) return false;

	m_detach = detach;
	return true;
}
bool DetachComponentCommand::Redo()
{
	auto [component, addable] = m_detach->DetachComponent();
	if (!component) return false;

	m_component = move(component);

	m_result = { m_component->Clone(), addable };

	return m_result.first != nullptr;
}

pair<unique_ptr<UIComponent>, UIComponent*> DetachComponentCommand::GetResult() noexcept
{
	auto& [detach, addable] = m_result;
	return { move(detach), addable };
}

//////////////////////////////////////////////////////////////////

SetRelativePositionCommand::SetRelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept :
	Command{ component }, m_record{ relativePos }
{}

bool SetRelativePositionCommand::Execute()
{
	auto prevPos = GetRelativePosition(GetComponent());
	ReturnIfFalse(prevPos.has_value());
	m_record.previous = *prevPos;
	ReturnIfFalse(SetRelativePosition(GetComponent(), m_record.current));

	return true;
}

bool SetRelativePositionCommand::Undo() { return SetRelativePosition(GetComponent(), m_record.previous); }
bool SetRelativePositionCommand::Redo() { return SetRelativePosition(GetComponent(), m_record.current); }

void SetRelativePositionCommand::PostMerge(unique_ptr<Command> other) noexcept
{
	auto otherCmd = static_cast<SetRelativePositionCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}

//////////////////////////////////////////////////////////////////

SetSizeCommand::SetSizeCommand(UIComponent* component, const XMUINT2& size) noexcept :
	Command{ component }, m_record{ size }
{}

bool SetSizeCommand::Execute()
{
	m_record.previous = UIComponentEx::GetSize(GetComponent());
	GetComponent()->ChangeSize(m_record.current);
	return true;
}

bool SetSizeCommand::Undo() { GetComponent()->ChangeSize(m_record.previous); return true; }
bool SetSizeCommand::Redo() { GetComponent()->ChangeSize(m_record.current);	return true; }

void SetSizeCommand::PostMerge(unique_ptr<Command> other) noexcept
{
	auto otherCmd = static_cast<SetSizeCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}

//////////////////////////////////////////////////////////////////

RenameCommand::RenameCommand(UIComponent* component, const string& name) noexcept :
	Command{ component }, m_record{ name }
{}

bool RenameCommand::Execute()
{
	m_record.previous = UIComponentEx::GetName(GetComponent());
	return Rename(GetComponent(), m_record.current);
}

bool RenameCommand::Undo() { return Rename(GetComponent(), m_record.previous); }
bool RenameCommand::Redo() { return Rename(GetComponent(), m_record.current); }

//////////////////////////////////////////////////////////////////

SetSourceCommand::SetSourceCommand(ImageGrid1* imageGrid1, const Rectangle& source) noexcept :
	Command{ imageGrid1 },
	m_imgGrid1{ imageGrid1 },
	m_record{ source }
{}

bool SetSourceCommand::Execute()
{
	m_record.previous = m_imgGrid1->GetSource();
	m_imgGrid1->SetSource(m_record.current);
	return true;
}

bool SetSourceCommand::Undo() { m_imgGrid1->SetSource(m_record.previous); return true; }
bool SetSourceCommand::Redo() { m_imgGrid1->SetSource(m_record.current); return true; }

void SetSourceCommand::PostMerge(unique_ptr<Command> other) noexcept
{
	auto otherCmd = static_cast<SetSourceCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}

//////////////////////////////////////////////////////////////////

SetFilenameCommand::SetFilenameCommand(const ImageGridVariant& imgGridVariant, 
	IRenderer* renderer, const wstring& filename) noexcept :
	//variant�ȿ� ������ UIComponent���� ��� ���� ���̶� visit�� ���� �ϳ� ���ͼ� ĳ���� �Ѵ�.
	//get<UIComponent*>�� ���� �ʾҴ�. ������ Ÿ�ӿ��� �𸣱� ������ ImageGrid1* �̷��� �־�߸� ������ �� �ִ�.
	Command{ visit([](auto* imgGrid) -> UIComponent* { return imgGrid; }, imgGridVariant) },
	m_imgGridVariant{ imgGridVariant },
	m_renderer{ renderer },
	m_record{ filename }
{}

wstring SetFilenameCommand::GetFilename() const noexcept
{
	return visit([](auto* imgGrid) -> wstring {
		if (imgGrid && imgGrid->GetFilename().has_value())
			return *imgGrid->GetFilename();
		return L"";
		}, m_imgGridVariant);
}

bool SetFilenameCommand::SetFilename(const wstring& filename)
{
	return visit([this, filename](auto* imgGrid) -> bool {
		if (imgGrid)
		{
			imgGrid->SetFilename(filename);
			return m_renderer->LoadComponent(imgGrid);
		}
		return false;
		}, m_imgGridVariant);
}

bool SetFilenameCommand::Execute()
{
	m_record.previous = GetFilename();
	return SetFilename(m_record.current);
}

bool SetFilenameCommand::Undo() { return SetFilename(m_record.previous); }
bool SetFilenameCommand::Redo() { return SetFilename(m_record.current); }

//////////////////////////////////////////////////////////////////

SetSourceAndDividerCommand::SetSourceAndDividerCommand(const ImageGrid39Variant& imgGridVariant,
	const SourceDivider& srcDivider) noexcept :
	Command{ visit([](auto* imgGrid) -> UIComponent* { return imgGrid; }, imgGridVariant) },
	m_imgGridVariant{ imgGridVariant },
	m_record{ srcDivider }
{}

optional<SourceDivider> SetSourceAndDividerCommand::GetSourceAndDivider() const noexcept
{
	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
		return (*imgGrid3)->GetSourceAnd2Divider();

	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
		return (*imgGrid9)->GetSourceAnd4Divider();

	return nullopt;
}

bool SetSourceAndDividerCommand::SetSourceAndDivider(const SourceDivider& srcDivider) noexcept
{
	if (auto imgGrid3 = get_if<ImageGrid3*>(&m_imgGridVariant); imgGrid3 && *imgGrid3)
		return (*imgGrid3)->SetSourceAnd2Divider(srcDivider);

	if (auto imgGrid9 = get_if<ImageGrid9*>(&m_imgGridVariant); imgGrid9 && *imgGrid9)
		return (*imgGrid9)->SetSourceAnd4Divider(srcDivider);

	return false;
}

bool SetSourceAndDividerCommand::Execute()
{
	auto srcDivider = GetSourceAndDivider();
	if (!srcDivider.has_value()) return false;

	m_record.previous = *srcDivider;
	return SetSourceAndDivider(m_record.current);
}

bool SetSourceAndDividerCommand::Undo() { return SetSourceAndDivider(m_record.previous); }
bool SetSourceAndDividerCommand::Redo() { return SetSourceAndDivider(m_record.current); }

void SetSourceAndDividerCommand::PostMerge(unique_ptr<Command> other) noexcept
{
	auto otherCmd = static_cast<SetSourceAndDividerCommand*>(other.get());
	m_record.current = otherCmd->m_record.current;
}