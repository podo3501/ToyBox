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
	return state;
}

bool MockInputManager::IsInputAction(MouseButton mouseButton, InputState inputState) noexcept
{
	if (mouseButton != MouseButton::Left) return false;
	return (m_inputState == inputState);
}

void MockInputManager::SetMouseState(int x, int y, InputState inputState) noexcept
{
	m_position = { x, y };
	m_inputState = inputState;
}