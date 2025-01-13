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
    //앞 키는 hold 하고 뒤에는 pressed를 해야 한다. 둘다 pressed를 하면 동시에 눌러야 하기 때문에 키가 안 먹힌다.
    bool isKeyHeld = manager.GetKeyboard().IsKeyHeld(key);
    bool isMouseButtonPressed = IsInputPressed(manager, mouseButton);

    // 두 조건이 모두 충족되면 true 반환
    return isKeyHeld && isMouseButtonPressed;
}

