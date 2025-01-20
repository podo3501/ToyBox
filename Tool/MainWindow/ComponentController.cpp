#include "pch.h"
#include "ComponentController.h"
#include "../Include/IRenderer.h"
#include "FloatingComponent.h"
#include "ComponentSelector.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Dialog.h"

ComponentController::~ComponentController() = default;
ComponentController::ComponentController(IRenderer* renderer,
	UIComponent* panel, const string& mainWndNam) noexcept :
	m_cmdList{ make_unique<CommandList>() },
	m_floater{ make_unique<FloatingComponent>(renderer, mainWndNam) },
	m_selector{ make_unique<ComponentSelector>(renderer, m_cmdList.get(), panel) }
{}

void ComponentController::SetPanel(UIComponent* panel) noexcept
{
	m_selector->SetPanel(panel);
}

void ComponentController::SetMainWindow(ImGuiWindow* mainWnd) noexcept
{
	m_selector->SetMainWindow(mainWnd);
}

bool ComponentController::CheckAttachComponent(const InputManager& inputManager) noexcept
{
	if (!m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::LeftShift, MouseButton::Left)) return false;

	AttachSelectedComponent(m_selector.get(), m_floater.get(), inputManager.GetMouse().GetOffsetPosition());

	return true;
}

bool ComponentController::CheckDetachComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::D, KeyState::Pressed)) return false;

	auto detachComponent = DetachSelectedComponent(m_selector.get());
	if (!detachComponent.has_value())
		return true;

	//Detach된 Component를 RenderTexture
	if (!m_floater->ComponentToFloating(move(detachComponent.value())))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to load the resource.");
		return true;
	}

	return true;
}

bool ComponentController::CheckDeleteComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::Delete, KeyState::Pressed)) return false;

	DetachSelectedComponent(m_selector.get());

	return true;
}

bool ComponentController::CheckUndoComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::LeftControl, Keyboard::Z)) return false;

	return m_cmdList->Undo();
}

bool ComponentController::CheckRedoComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::LeftControl, Keyboard::Y)) return false;

	return m_cmdList->Redo();
}

bool ComponentController::CheckCloneComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::B, KeyState::Pressed)) return false;
	UIComponent* component = m_selector->GetComponent();
	if (!component) return false;

	if(!component->IsDetachable())
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Cannot be cloned. Could it be a component that cannot be detached?");
		return true;
	}

	auto clone = component->Clone();
	if (!m_floater->ComponentToFloating(move(clone)))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to load the resource.");
		return true;
	}

	return true;
}

bool ComponentController::Update(const InputManager& inputManager) noexcept
{
	CheckDetachComponent(inputManager);
	CheckDeleteComponent(inputManager);
	CheckCloneComponent(inputManager);
	CheckUndoComponent(inputManager);
	CheckRedoComponent(inputManager);
	if (!CheckAttachComponent(inputManager)) //Attach 할때 select 되는경우가 있어서
		m_selector->Update(inputManager);	
	
	m_floater->Excute();

	return true;
}

void ComponentController::Render()
{
	if (!Tool::Dialog::IsOpenDialog())
		m_floater->Render();
	m_selector->Render();
}
