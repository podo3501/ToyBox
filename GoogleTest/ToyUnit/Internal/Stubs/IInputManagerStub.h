#pragma once
#include "Shared/System/Public/IInputManager.h"

struct IInputManagerStub : public IInputManager
{
    virtual ~IInputManagerStub() = default;
    virtual void SetMouseStartOffset(const DirectX::XMINT2& offset) noexcept override {};
    virtual void Update() noexcept override {};
    virtual MouseState GetMouseState() const noexcept override { return {}; }
    virtual const DirectX::XMINT2& GetPosition() const noexcept override { return m_pos; }

    virtual bool IsInputAction(DirectX::Keyboard::Keys key, InputState inputState) noexcept override { return false; }
    virtual bool IsInputAction(DirectX::Keyboard::Keys key, MouseButton mouseButton) noexcept override { return false; }
    virtual bool IsInputAction(DirectX::Keyboard::Keys firstKey, DirectX::Keyboard::Keys secondKey) noexcept override { return false; }
    virtual bool IsInputAction(MouseButton mouseButton, InputState keyState) noexcept override { return false; }
    virtual void ResetMouseWheelValue() noexcept override {};
    virtual int GetMouseWheelValue() noexcept override { return 0; }

private:
    DirectX::XMINT2 m_pos{};
};