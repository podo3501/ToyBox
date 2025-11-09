#pragma once
#include "Stubs/IInputManagerStub.h"

class MockInputManager : public IInputManagerStub
{
public:
	virtual const DirectX::XMINT2& GetPosition() const noexcept override;

	virtual MouseState GetMouseState() const noexcept override;
	virtual int GetMouseWheelValue() noexcept override { return m_wheelValue; }
	virtual bool IsInputAction(MouseButton mouseButton, InputState keyState) noexcept override;

	void SetMouseState(const XMINT2& pos, InputState inputState = InputState::Up) noexcept;
	void SetMouseState(int x, int y, InputState inputState = InputState::Up) noexcept;
	void SetMouseWheelValue(int wheelValue) noexcept;
	
private:
	XMINT2 m_position{};
	int m_wheelValue{ 0 };
	InputState m_inputState{};
};
