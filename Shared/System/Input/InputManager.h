#pragma once
#include "../Public/IInputManager.h"
#include "DirectXTK12/Keyboard.h"
#include "DirectXTK12/Mouse.h"

class InputManager : public IInputManager
{
public:
    ~InputManager() = default;
    InputManager() = default;
    void Initialize(HWND hwnd);

    virtual void SetMouseStartOffset(const XMINT2& offset) noexcept override;
    virtual void Update() noexcept override;
    virtual MouseState GetMouseState() const noexcept override;
    virtual const XMINT2& GetPosition() const noexcept override;

    virtual bool IsInputAction(Keyboard::Keys key, InputState inputState) noexcept override;
    virtual bool IsInputAction(Keyboard::Keys key, MouseButton mouseButton) noexcept override;
    virtual bool IsInputAction(Keyboard::Keys firstKey, Keyboard::Keys secondKey) noexcept override;
    virtual bool IsInputAction(MouseButton mouseButton, InputState keyState) noexcept override;
    virtual void ResetMouseWheelValue() noexcept override;
    virtual int GetMouseWheelValue() noexcept override;

private:
    void SetPosition(XMINT2 position) noexcept { m_position = position; }

    Keyboard::KeyboardStateTracker m_keyboardTracker{};
    Mouse::ButtonStateTracker m_mouseTracker{};

    XMINT2 m_startOffset{};
    XMINT2 m_position{};
    int m_lastMouseWheelValue{ 0 };
};