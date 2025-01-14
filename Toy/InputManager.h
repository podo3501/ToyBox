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

    static bool m_instantiated; //�� Ŭ������ Keyboard, Mouse�� �̱����̱� ������ ���� �ϳ��� ��������� �Ѵ�.
};

/////////////////////////////////////////////////////////////////////////
// Mouse

//�ӵ��� ���� �Լ� �����ͷ� ������.
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
    //�� enum�� ���� �����ϴ�.
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

//�ӵ��� ���ؼ� function��ſ� �Լ� �����͸� �����
using ActionFunc = bool(*)(const KeyboardTracker&, Keyboard::Keys);
constexpr ActionFunc KeyActions[] = 
{
    [](const KeyboardTracker&, Keyboard::Keys) { return false; },   //Ű���忡�� UP�� ���� �Լ��� ����.
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
    //�� Ű�� hold �ϰ� �ڿ��� pressed�� �ؾ� �Ѵ�. �Ѵ� pressed�� �ϸ� ���ÿ� ������ �ϱ� ������ Ű�� �� ������.
    return IsInputAction(manager, key, KeyState::Held) && 
        IsInputAction(manager, mouseButton, KeyState::Pressed);
}