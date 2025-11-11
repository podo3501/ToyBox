#pragma once
#include "Internal/MockInputManager.h"
#include "Shared/System/StepTimer.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/Locator/InputLocator.h"

class UIComponentManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	void SimulateMouse(const XMINT2& pos, InputState state) noexcept;
	void SimulateMouse(int x, int y, InputState state) noexcept;
	void SimulateMouse(const XMINT2& pos, int wheelValue) noexcept;
	void SimulateMouse(int x, int y, int wheelValue) noexcept;
	void SimulateClick(const XMINT2& startPos) noexcept;
	void SimulateDrag(const XMINT2& startPos, const XMINT2& endPos) noexcept;

	unique_ptr<MockInputManager> m_input;
	unique_ptr<UIComponentManager> m_componentManager;
};

void UIComponentManagerT::SetUp()
{
	m_input = make_unique<MockInputManager>();
	InputLocator::Provide(m_input.get());

	m_componentManager = make_unique<UIComponentManager>();
	Locator<UIComponentManager>::Provide(m_componentManager.get());
}

void UIComponentManagerT::SimulateMouse(const XMINT2& pos, InputState state) noexcept
{
	SimulateMouse(pos.x, pos.y, state);
}

void UIComponentManagerT::SimulateMouse(int x, int y, InputState state) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, state);
	//m_uiModule->Update(timer);
}

void UIComponentManagerT::SimulateMouse(const XMINT2& pos, int wheelValue) noexcept
{
	SimulateMouse(pos.x, pos.y, wheelValue);
}

void UIComponentManagerT::SimulateMouse(int x, int y, int wheelValue) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, InputState::Up);
	m_input->SetMouseWheelValue(wheelValue);
	//m_uiModule->Update(timer);
}

void UIComponentManagerT::SimulateClick(const XMINT2& startPos) noexcept
{
	SimulateDrag(startPos, startPos);
}

void UIComponentManagerT::SimulateDrag(const XMINT2& startPos, const XMINT2& endPos) noexcept
{
	SimulateMouse(startPos, InputState::Pressed);
	SimulateMouse(endPos, InputState::Held);
	SimulateMouse(endPos, InputState::Released);
}