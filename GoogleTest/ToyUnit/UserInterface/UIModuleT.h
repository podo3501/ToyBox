#pragma once
#include "BaseModuleT.h"
#include "Mocks/MockInputManager.h"
#include "Toy/Locator/InputLocator.h"
#include "Shared/System/StepTimer.h"

class UIModuleT : public BaseModuleT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	void SimulateMouse(const XMINT2& pos, InputState state) noexcept;
	void SimulateMouse(int x, int y, InputState state) noexcept;
	void SimulateClick(const XMINT2& startPos) noexcept;
	void SimulateClick(const XMINT2& startPos, const XMINT2& endPos) noexcept;

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