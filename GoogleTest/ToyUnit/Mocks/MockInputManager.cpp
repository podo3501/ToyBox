#include "pch.h"
#include "MockInputManager.h"
#include "Shared/System/Input.h"

const DirectX::XMINT2& MockInputManager::GetPosition() const noexcept
{
	return m_position;
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