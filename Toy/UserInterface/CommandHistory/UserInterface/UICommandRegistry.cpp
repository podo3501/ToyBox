#include "pch.h"
#include "UICommandRegistry.h"
#include "IRenderer.h"
#include "../../UIModule.h"
#include "../../UIComponent/Components/PatchTexture/PatchTexture.h"
#include "../../UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../../UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "../../UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "../../UIComponent/Components/TextureSwitcher.h"
#include "../../UIComponent/Components/TextArea.h"

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
	//if(!CompareUniquePtr(m_result.first, m_component)) //Undo, Redo를 계속하면 m_result에 값이 있어서 소멸자가 호출된다. 그래서 texture의 reference count가 꼬이게 된다.
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
	UIEx(GetTarget()).RenameRegion(m_record.current);
	return true;
}

bool RenameRegionCommand::Undo() { UIEx(GetTarget()).RenameRegion(m_record.previous); return true; }
bool RenameRegionCommand::Redo() { UIEx(GetTarget()).RenameRegion(m_record.current); return true; }

//////////////////////////////////////////////////////////////////

RenameCommand::RenameCommand(UIComponent* component, const string& name) noexcept :
	UICommand{ component }, m_record{ name }
{}

bool RenameCommand::Execute()
{
	m_record.previous = GetTarget()->GetName();
	return UIEx(GetTarget()).Rename(m_record.current);
}

bool RenameCommand::Undo() { return UIEx(GetTarget()).Rename(m_record.previous); }
bool RenameCommand::Redo() { return UIEx(GetTarget()).Rename(m_record.current); }

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

//////////////////////////////////////////////////////////////////

FitToTextureSourceCommand::FitToTextureSourceCommand(PatchTexture* patchTex) noexcept :
	UICommand{ patchTex },
	m_commandID{ UICommandID::FitToTextureSource }
{}

FitToTextureSourceCommand::FitToTextureSourceCommand(TextureSwitcher* texSwitcher) noexcept :
	UICommand{ texSwitcher },
	m_commandID{ UICommandID::FitToTextureSourceTS }
{}

template<typename FuncT>
bool FitToTextureSourceCommand::WithTarget(FuncT&& Func)
{
	switch (m_commandID) {
	case UICommandID::FitToTextureSource: return Func(static_cast<PatchTexture*>(GetTarget()));
	case UICommandID::FitToTextureSourceTS: return Func(static_cast<TextureSwitcher*>(GetTarget()));
	default: return false;
	}
}

bool FitToTextureSourceCommand::Execute()
{
	return WithTarget([this](auto* target) {
		m_size = target->GetSize();
		return target->FitToTextureSource();
		});
}

bool FitToTextureSourceCommand::Undo() { return WithTarget([this](auto* t) { return t->ChangeSize(m_size); }); }
bool FitToTextureSourceCommand::Redo() { return WithTarget([](auto* t) { return t->FitToTextureSource(); }); }

//////////////////////////////////////////////////////////////////

ChangeBindKeyCommand::ChangeBindKeyCommand(PatchTextureStd* patchTexStd, TextureResourceBinder* resBinder, const string& key) noexcept :
	UICommand{ patchTexStd },
	m_commandID{ UICommandID::ChangeBindKey },
	m_resBinder{ resBinder },
	m_record{ key }
{}

ChangeBindKeyCommand::ChangeBindKeyCommand(TextureSwitcher* texSwitcher, TextureResourceBinder* resBinder, const string& key) noexcept :
	UICommand{ texSwitcher },
	m_commandID{ UICommandID::ChangeBindKeyTS },
	m_resBinder{ resBinder },
	m_record{ key }
{}

template<typename FuncT>
bool ChangeBindKeyCommand::WithTarget(FuncT&& Func)
{
	switch (m_commandID) {
	case UICommandID::ChangeBindKey: return Func(static_cast<PatchTextureStd*>(GetTarget()));
	case UICommandID::ChangeBindKeyTS: return Func(static_cast<TextureSwitcher*>(GetTarget()));
	default: return false;
	}
}

bool ChangeBindKeyCommand::Execute()
{
	return WithTarget([this](auto* target) {
		m_record.previous = target->GetBindKey();
		m_prevSize = target->GetSize();
		return target->ChangeBindKey(m_resBinder, m_record.current);
		});
}

bool ChangeBindKeyCommand::Undo() {
	return WithTarget([this](auto* t) { 
		ReturnIfFalse(t->ChangeBindKey(m_resBinder, m_record.previous));
		return t->ChangeSize(m_prevSize);
		}); 
}
bool ChangeBindKeyCommand::Redo() { return WithTarget([this](auto* t) { return t->ChangeBindKey(m_resBinder, m_record.current); }); }