#include "pch.h"
#include "MockInputManager.h"

const DirectX::XMINT2& MockInputManager::GetPosition() const noexcept
{
	return m_position;
}

MouseState MockInputManager::GetMouseState() const noexcept
{
	MouseState state;
	state.pos = m_position;
	state.leftButton = (m_inputState == InputState::Pressed || m_inputState == InputState::Held);
	state.wheelValue = m_wheelValue;
	return state;
}

bool MockInputManager::IsInputAction(MouseButton mouseButton, InputState inputState) noexcept
{
	if (mouseButton != MouseButton::Left) return false;
	return (m_inputState == inputState);
}

void MockInputManager::SetMouseState(const XMINT2& pos, InputState inputState) noexcept
{
	m_position = pos;
	m_inputState = inputState;
}

void MockInputManager::SetMouseState(int x, int y, InputState inputState) noexcept
{
	SetMouseState({ x, y }, inputState);
}

void MockInputManager::SetMouseWheelValue(int wheelValue) noexcept
{
	m_wheelValue = wheelValue;
}