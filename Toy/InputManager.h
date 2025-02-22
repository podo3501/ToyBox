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

//�� Ŭ������ ���� Ŭ������ �Ҹ��ڰ� ȣ����� �ʴ´�. �׷��� TDD������ �������� �׽�Ʈ�� �Ҷ� �� Ŭ������
// �Ҹ���� �ʰ� ���α׷��� ����ɶ� �ѹ� �Ҹ�ȴ�. ��������� �Ҹ��ϴ� ����� ������, unique_ptr��
//����ϴ� ����ε� cache�� ������ ���� �� �־ �� Ŭ�������� �����͸� ����ϰ� ���� �ʾҴ�.
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

//�ӵ��� ���ؼ� function��ſ� �Լ� �����͸� �����
using ActionFunc = bool(*)(const KeyboardTracker&, Keyboard::Keys);
constexpr ActionFunc KeyActions[] = 
{
    [](const KeyboardTracker&, Keyboard::Keys) { return false; },   //Ű���忡�� UP�� ���� �Լ��� ����.
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
    //�� Ű�� hold �ϰ� �ڿ��� pressed�� �ؾ� �Ѵ�. �Ѵ� pressed�� �ϸ� ���ÿ� ������ �ϱ� ������ Ű�� �� ������.
    return IsInputAction(key, KeyState::Held) && IsInputAction(mouseButton, KeyState::Pressed);
}

inline bool IsInputAction(Keyboard::Keys firstKey, Keyboard::Keys secondKey) noexcept
{
    bool firstKeyHeld = IsInputAction(firstKey, KeyState::Held);
    bool secondKeyPressed = IsInputAction(secondKey, KeyState::Pressed);
    
    return firstKeyHeld && secondKeyPressed; 
    //������ ������ �ʹ� ���� �̺�Ʈ�� ����. �׷��� �ð� ������ �� �����̾��µ� ���������� ������ ��
    //���� �ΰ��� �ð������� ��� �ѹ��� �����Ⱑ ��ư� �ȴ�. �ϴ��� �ϳ��� �ϴ� �ɷ� �ϰ� ���Ŀ� ��
    //�κ��� �����ؼ� ��� Ű�� �������� ���ݰ�, ������ �� ������ ������ �� ������ ������ �����ϰԲ� ��������.
}