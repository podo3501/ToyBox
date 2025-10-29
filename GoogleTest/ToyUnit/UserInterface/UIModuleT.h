#pragma once
#include "Mocks/MockRenderer.h"
#include "Mocks/MockInputManager.h"
#include "Toy/Locator/InputLocator.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/Utils/GeometryExt.h"

class UIModuleT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	void SimulateMouse(const XMINT2& pos, InputState state) noexcept;
	void SimulateMouse(int x, int y, InputState state) noexcept;
	void SimulateClick(const XMINT2& startPos) noexcept;
	void SimulateClick(const XMINT2& startPos, const XMINT2& endPos) noexcept;

	unique_ptr<Environment> m_environment = InitializeEnvironment(L"", { 800.f, 600.f });
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<MockInputManager> m_input;
	unique_ptr<UIModule> m_uiModule;
	UIComponent* m_main{ nullptr };
};

void UIModuleT::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	m_renderer = make_unique<MockRenderer>();
	m_renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});
	m_input = make_unique<MockInputManager>();
	InputLocator::Provide(m_input.get());

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	m_uiModule = CreateUIModule(layout, "Main", m_renderer.get());
	auto resBinder = m_uiModule->GetTexResBinder();
	m_main = m_uiModule->GetMainPanel();
}

void UIModuleT::SimulateMouse(const XMINT2& pos, InputState state) noexcept
{
	SimulateMouse(pos.x, pos.y, state);
}

void UIModuleT::SimulateMouse(int x, int y, InputState state) noexcept
{
	m_input->SetMouseState(x, y, state);
	m_uiModule->UpdateMouseState();
}

void UIModuleT::SimulateClick(const XMINT2& startPos) noexcept
{
	SimulateClick(startPos, startPos);
}

void UIModuleT::SimulateClick(const XMINT2& startPos, const XMINT2& endPos) noexcept
{
	SimulateMouse(startPos, InputState::Pressed);
	SimulateMouse(endPos, InputState::Held);
	SimulateMouse(endPos, InputState::Released);
}