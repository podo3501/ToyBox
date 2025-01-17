#include "pch.h"
#include "ComponentController.h"
#include "../Include/IRenderer.h"
#include "FloatingComponent.h"
#include "ComponentSelector.h"
#include "../Dialog.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"

ComponentController::~ComponentController() = default;
ComponentController::ComponentController(IRenderer* renderer,
	UIComponent* panel, const string& mainWndNam) noexcept :
	m_floater{ make_unique<FloatingComponent>(renderer, mainWndNam) },
	m_selector{ make_unique<ComponentSelector>(renderer, panel) }
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

	//DetachµÈ Component¸¦ RenderTexture
	if (!m_floater->DetachToFloating(move(detachComponent.value())))
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

bool ComponentController::Update(const InputManager& inputManager) noexcept
{
	CheckDetachComponent(inputManager);
	CheckDeleteComponent(inputManager);
	if (!CheckAttachComponent(inputManager))
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
