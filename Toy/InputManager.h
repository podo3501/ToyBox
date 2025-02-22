#pragma once

class MouseTracker : public Mouse::ButtonStateTracker
{
public:
    XMINT2 GetPosition() const noexcept;
};

class KeyboardTracker : public Keyboard::KeyboardStateTracker
{
public:
    bool __cdecl IsKeyHeld(Keyboard::Keys key) const noexcept;
};

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

//이 클래스는 정적 클래스라 소멸자가 호출되지 않는다. 그래서 TDD에서는 여러개의 테스트를 할때 이 클래스는
// 소멸되지 않고 프로그램이 종료될때 한번 소멸된다. 명시적으로 소멸하는 방법이 있으나, unique_ptr을
//사용하는 방법인데 cache에 영향이 있을 수 있어서 이 클래스에는 포인터를 사용하고 싶지 않았다.
class InputManager
{
public:
    static void Initialize(HWND hwnd);
    static void Update() noexcept;
    
    static inline const KeyboardTracker& GetKeyboard() noexcept { return m_keyboardTracker; }
    static inline const MouseTracker& GetMouse() noexcept { return m_mouseTracker; }
    static inline void SetMouseStartOffset(const XMINT2& offset) noexcept { m_startOffset = offset; }

private:
    static KeyboardTracker m_keyboardTracker;
    static Keyboard m_keyboard;

    static MouseTracker m_mouseTracker;
    static Mouse m_mouse;
    static optional<XMINT2> m_startOffset;
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

inline bool IsInputAction(MouseButton mouseButton, KeyState keyState) noexcept
{
    const auto& mouseTracker = InputManager::GetMouse();
    return IsMouseButtonState(mouseTracker, mouseButton, GetMouseKeyState(keyState));
}

static int LastMouseWheelValue{ 0 };
inline void ResetMouseWheelValue() noexcept
{
    LastMouseWheelValue = InputManager::GetMouse().GetLastState().scrollWheelValue;
}

inline int GetMouseWheelValue() noexcept
{
    const auto& mouseTracker = InputManager::GetMouse();
    const int& curWheelValue = mouseTracker.GetLastState().scrollWheelValue;
    int delta = ( curWheelValue - LastMouseWheelValue ) / 120 ;
    LastMouseWheelValue = curWheelValue;

    return delta;
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

inline bool IsInputAction(Keyboard::Keys key, KeyState keyState) noexcept
{
    const auto& keyboard = InputManager::GetKeyboard();
    return KeyActions[static_cast<size_t>(keyState)](keyboard, key);
}

inline bool IsInputAction(Keyboard::Keys key, MouseButton mouseButton) noexcept
{
    //앞 키는 hold 하고 뒤에는 pressed를 해야 한다. 둘다 pressed를 하면 동시에 눌러야 하기 때문에 키가 안 먹힌다.
    return IsInputAction(key, KeyState::Held) && IsInputAction(mouseButton, KeyState::Pressed);
}

inline bool IsInputAction(Keyboard::Keys firstKey, Keyboard::Keys secondKey) noexcept
{
    bool firstKeyHeld = IsInputAction(firstKey, KeyState::Held);
    bool secondKeyPressed = IsInputAction(secondKey, KeyState::Pressed);
    
    return firstKeyHeld && secondKeyPressed; 
    //눌리고 있을때 너무 빨리 이벤트가 들어간다. 그래서 시간 지연을 줄 생각이었는데 눌렀을때와 눌리고 있
    //을때 두개의 시간간격이 없어서 한번씩 누르기가 어렵게 된다. 일단은 하나씩 하는 걸로 하고 추후에 이
    //부분을 개선해서 계속 키를 눌렀을때 간격과, 눌렀을 때 눌리고 있을때 이 사이의 간격을 지정하게끔 구현하자.
}