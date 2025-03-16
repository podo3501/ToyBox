#include "pch.h"
#include "ComponentController.h"
#include "../Include/IRenderer.h"
#include "FloatingComponent.h"
#include "ComponentSelector.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Dialog.h"
#include "../Utility.h"

ComponentController::~ComponentController() = default;
ComponentController::ComponentController(IRenderer* renderer, TextureSourceBinder* sourceBinder,
	UIComponent* panel, const string& mainWndNam) noexcept :
	m_mainWnd{ nullptr },
	m_cmdList{ make_unique<CommandList>() },
	m_floater{ make_unique<FloatingComponent>(renderer, sourceBinder, mainWndNam) },
	m_selector{ make_unique<ComponentSelector>(renderer, m_cmdList.get(), panel) }
{}

void ComponentController::SetPanel(UIComponent* panel) noexcept
{
	m_selector->SetPanel(panel);
}

void ComponentController::SetMainWindow(ImGuiWindow* mainWnd) noexcept
{
	m_mainWnd = mainWnd;
	m_selector->SetMainWindow(mainWnd);
}

bool ComponentController::CheckAttachComponent() noexcept
{
	if (!m_mainWnd) return false;
	if (!m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::LeftShift, MouseButton::Left)) return false;

	const XMINT2& mousePosition = InputManager::GetMouse().GetPosition();
	AttachSelectedComponent(m_cmdList.get(), m_selector.get(), m_floater.get(), mousePosition);

	return true;
}

bool ComponentController::CheckDetachComponent() noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::D, KeyState::Pressed)) return false;

	auto detachComponent = DetachSelectedComponent(m_cmdList.get(), m_selector.get());
	if (!detachComponent)
		return true;

	//DetachµÈ Component¸¦ RenderTexture
	if (!m_floater->ComponentToFloating(move(detachComponent)))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to load the resource.");
		return true;
	}

	return true;
}

bool ComponentController::CheckDeleteComponent() noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::Delete, KeyState::Pressed)) return false;

	DetachSelectedComponent(m_cmdList.get(), m_selector.get());

	return true;
}

bool ComponentController::CheckUndoComponent() noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::LeftControl, Keyboard::Z)) return false;

	return m_cmdList->Undo();
}

bool ComponentController::CheckRedoComponent() noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::LeftControl, Keyboard::Y)) return false;

	return m_cmdList->Redo();
}

bool ComponentController::CheckCloneComponent() noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(Keyboard::B, KeyState::Pressed)) return false;
	UIComponent* component = m_selector->GetComponent();
	if (!component) return false;

	auto clone = component->Clone();
	if(!clone)
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Cannot be cloned. Could it be a component that cannot be detached?");
		return true;
	}
	
	if (!m_floater->ComponentToFloating(move(clone)))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to load the resource.");
		return true;
	}

	return true;
}

void ComponentController::SetActive(bool active) noexcept
{
	m_active = active;
}

bool ComponentController::ExecuteShortcutKeyCommands() noexcept
{
	if (!IsWindowFocus(m_mainWnd)) return false;

	return CheckDetachComponent() ||
		CheckDeleteComponent() ||
		CheckCloneComponent() ||
		CheckUndoComponent() ||
		CheckRedoComponent() ||
		CheckAttachComponent();
}

bool ComponentController::Update() noexcept
{
	if (!m_active) return true;

	if(!ExecuteShortcutKeyCommands())
		m_selector->Update();	
	m_floater->Excute();

	return true;
}

void ComponentController::Render()
{
	if (!m_active) return;

	if (!Tool::Dialog::IsOpenDialog())
		m_floater->Render();
	m_selector->Render();
}
