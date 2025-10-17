#pragma once
#include "Stubs/IInputManagerStub.h"

class MockInputManager : public IInputManagerStub
{
public:
	virtual const DirectX::XMINT2& GetPosition() const noexcept override;

	virtual bool IsInputAction(MouseButton mouseButton, InputState keyState) noexcept override;

	void SetMouseState(int x, int y, InputState inputState) noexcept;

private:
	XMINT2 m_position{};
	InputState m_inputState{};
};
