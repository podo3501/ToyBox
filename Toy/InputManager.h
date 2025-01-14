#pragma once
#include"HelperClass.h"

enum class MouseButton
{
    Left,
    Right,
    Middle
};

enum class KeyState
{
    Up,
    Held,
    Released,
    Pressed,
};

namespace DirectX
{
    class Keyboard::KeyboardStateTracker;
}

class InputManager
{
public:
    InputManager() = delete;
    InputManager(HWND hwnd);
    ~InputManager();

    void Update();
    void Update(const Mouse::State& state);

    inline const KeyboardTracker& GetKeyboard() const noexcept { return m_keyboardTracker; }
    inline const MouseTracker& GetMouse() const noexcept { return m_mouseTracker; }
    inline MouseTracker& GetMouse() noexcept { return m_mouseTracker; }

private:
    KeyboardTracker m_keyboardTracker;
    Keyboard m_keyboard;

    MouseTracker m_mouseTracker;
    Mouse m_mouse;

    static bool m_instantiated; //이 클래스는 Keyboard, Mouse가 싱글턴이기 때문에 오직 하나만 만들어져야 한다.
};

/////////////////////////////////////////////////////////////////////////
// Mouse

//속도를 위해 함수 포인터로 구현함.
using ButtonStateChecker = bool(*)(const MouseTracker&, Mouse::ButtonStateTracker::ButtonState);
static bool CheckLeftButton(const MouseTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.leftButton == state;
}

static bool CheckRightButton(const MouseTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.rightButton == state;
}

static bool CheckMiddleButton(const MouseTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.middleButton == state;
}

static bool IsMouseButtonState(const MouseTracker& mouseTracker, MouseButton mouseButton, Mouse::ButtonStateTracker::ButtonState state) noexcept
{
    constexpr ButtonStateChecker ButtonCheckers[] = {
        &CheckLeftButton,
        &CheckRightButton,
        &CheckMiddleButton
    };

    return ButtonCheckers[static_cast<size_t>(mouseButton)](mouseTracker, state);
}

inline Mouse::ButtonStateTracker::ButtonState GetMouseKeyState(KeyState keyState)
{
    //두 enum의 값이 동일하다.
    return static_cast<Mouse::ButtonStateTracker::ButtonState>(keyState);
}

inline bool IsInputAction(const MouseTracker& mouseTracker, MouseButton mouseButton, KeyState keyState) noexcept
{
    return IsMouseButtonState(mouseTracker, mouseButton, GetMouseKeyState(keyState));
}

inline bool IsInputAction(const InputManager& inputManager, MouseButton mouseButton, KeyState keyState) noexcept
{
    const auto& mouse = inputManager.GetMouse();
    return IsInputAction(mouse, mouseButton, keyState);
}

/////////////////////////////////////////////////////////////////////////
// Keyboard

//속도를 위해서 function대신에 함수 포인터를 사용함
using ActionFunc = bool(*)(const KeyboardTracker&, Keyboard::Keys);
constexpr ActionFunc KeyActions[] = 
{
    [](const KeyboardTracker&, Keyboard::Keys) { return false; },   //키보드에는 UP에 대한 함수가 없음.
    [](const KeyboardTracker& keyboard, Keyboard::Keys key) { return keyboard.IsKeyHeld(key); },
    [](const KeyboardTracker& keyboard, Keyboard::Keys key) { return keyboard.IsKeyReleased(key); },
    [](const KeyboardTracker& keyboard, Keyboard::Keys key) { return keyboard.IsKeyPressed(key); }
};

inline bool IsInputAction(const InputManager& manager, Keyboard::Keys key, KeyState keyState) noexcept
{
    const auto& keyboard = manager.GetKeyboard();
    return KeyActions[static_cast<size_t>(keyState)](keyboard, key);
}

inline bool IsInputAction(const InputManager& manager, Keyboard::Keys key, MouseButton mouseButton) noexcept
{
    //앞 키는 hold 하고 뒤에는 pressed를 해야 한다. 둘다 pressed를 하면 동시에 눌러야 하기 때문에 키가 안 먹힌다.
    return IsInputAction(manager, key, KeyState::Held) && 
        IsInputAction(manager, mouseButton, KeyState::Pressed);
}