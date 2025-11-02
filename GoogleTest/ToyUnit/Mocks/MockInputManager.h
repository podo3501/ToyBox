#pragma once
#include "Stubs/IInputManagerStub.h"

class MockInputManager : public IInputManagerStub
{
public:
	virtual const DirectX::XMINT2& GetPosition() const noexcept override;

	virtual MouseState GetMouseState() const noexcept override;
	virtual bool IsInputAction(MouseButton mouseButton, InputState keyState) noexcept override;

	void SetMouseState(const XMINT2& pos, InputState inputState = InputState::Up) noexcept;
	void SetMouseState(int x, int y, InputState inputState = InputState::Up) noexcept;
	

private:
	XMINT2 m_position{};
	InputState m_inputState{};
};
