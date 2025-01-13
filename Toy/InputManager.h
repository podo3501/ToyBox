#pragma once
#include"HelperClass.h"

enum class MouseButton
{
    Left,
    Right,
    Middle
};

namespace DirectX
{
    class Keyboard::KeyboardStateTracker;
}

class InputManager
{
public:
    InputManager(HWND hwnd);
    InputManager() = default;
    ~InputManager();

    void Update();
    void Update(const Mouse::State& state);

    inline const KeyboardTracker& GetKeyboard() const noexcept { return m_keyboardTracker; }
    inline const MouseTracker& GetMouse() const noexcept { return m_mouseTracker; }
    inline MouseTracker& GetMouse() noexcept { return m_mouseTracker; }

private:
    KeyboardTracker m_keyboardTracker;
    DirectX::Keyboard m_keyboard;

    MouseTracker m_mouseTracker;
    Mouse m_mouse;
};

static bool IsMouseButtonState(const MouseTracker& mouseTracker, MouseButton mouseButton, Mouse::ButtonStateTracker::ButtonState state) noexcept
{
    switch (mouseButton)
    {
    case MouseButton::Left: return (mouseTracker.leftButton == state);
    case MouseButton::Right: return (mouseTracker.rightButton == state);
    case MouseButton::Middle: return (mouseTracker.middleButton == state);
    default:
        return false;
    }
}

inline bool IsInputPressed(const MouseTracker& mouseTracker, MouseButton mouseButton) noexcept
{
    return IsMouseButtonState(mouseTracker, mouseButton, Mouse::ButtonStateTracker::PRESSED);
}

inline bool IsInputReleased(const MouseTracker& mouseTracker, MouseButton mouseButton) noexcept
{
    return IsMouseButtonState(mouseTracker, mouseButton, Mouse::ButtonStateTracker::RELEASED);
}

inline bool IsInputPressed(const InputManager& manager, MouseButton mouseButton) noexcept
{
    return IsInputPressed(manager.GetMouse(), mouseButton);
}

inline bool IsInputReleased(const InputManager& manager, MouseButton mouseButton) noexcept
{
    return IsInputReleased(manager.GetMouse(), mouseButton);
}

inline bool IsInputPressed(const InputManager& manager, Keyboard::Keys key) noexcept
{
    return manager.GetKeyboard().IsKeyPressed(key);
}

inline bool IsInputPressed(const InputManager& manager, Keyboard::Keys key, MouseButton mouseButton) noexcept
{
    //�� Ű�� hold �ϰ� �ڿ��� pressed�� �ؾ� �Ѵ�. �Ѵ� pressed�� �ϸ� ���ÿ� ������ �ϱ� ������ Ű�� �� ������.
    bool isKeyHeld = manager.GetKeyboard().IsKeyHeld(key);
    bool isMouseButtonPressed = IsInputPressed(manager, mouseButton);

    // �� ������ ��� �����Ǹ� true ��ȯ
    return isKeyHeld && isMouseButtonPressed;
}

