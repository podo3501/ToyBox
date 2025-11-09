#pragma once
#include "BaseModuleT.h"
#include "Internal/MockInputManager.h"
#include "Toy/Locator/InputLocator.h"
#include "Shared/System/StepTimer.h"

class UIModuleT : public BaseModuleT
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
};

void UIModuleT::SetUp()
{
	BaseModuleT::SetUp();

	m_input = make_unique<MockInputManager>();
	InputLocator::Provide(m_input.get());
}

void UIModuleT::SimulateMouse(const XMINT2& pos, InputState state) noexcept
{
	SimulateMouse(pos.x, pos.y, state);
}

void UIModuleT::SimulateMouse(int x, int y, InputState state) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, state);
	m_uiModule->Update(timer);
}

void UIModuleT::SimulateMouse(const XMINT2& pos, int wheelValue) noexcept
{
	SimulateMouse(pos.x, pos.y, wheelValue);
}

void UIModuleT::SimulateMouse(int x, int y, int wheelValue) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, InputState::Up);
	m_input->SetMouseWheelValue(wheelValue);
	m_uiModule->Update(timer);
}

void UIModuleT::SimulateClick(const XMINT2& startPos) noexcept
{
	SimulateDrag(startPos, startPos);
}

void UIModuleT::SimulateDrag(const XMINT2& startPos, const XMINT2& endPos) noexcept
{
	SimulateMouse(startPos, InputState::Pressed);
	SimulateMouse(endPos, InputState::Held);
	SimulateMouse(endPos, InputState::Released);
}