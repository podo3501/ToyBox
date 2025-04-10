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
	m_component = move(component); //������ �����ϰ� Ŭ���� ������ 
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
	if(!CompareUniquePtr(m_result.first, m_component)) //Undo, Redo�� ����ϸ� m_result�� ���� �־ �Ҹ��ڰ� ȣ��ȴ�. �׷��� texture�� reference count�� ���̰� �ȴ�.
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