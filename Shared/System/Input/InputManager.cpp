#include "pch.h"
#include "InputManager.h"
#include "../Input.h"

//�ӵ��� ���ؼ� function��ſ� �Լ� �����͸� �����
using InputActionFunc = bool(*)(const Keyboard::KeyboardStateTracker&, Keyboard::Keys);
constexpr InputActionFunc KeyboardActions[] =
{
    [](const Keyboard::KeyboardStateTracker&, Keyboard::Keys) { return false; },   //Ű���忡�� UP�� ���� �Լ��� ����.
    [](const Keyboard::KeyboardStateTracker& keyboard, Keyboard::Keys key) { return keyboard.GetLastState().IsKeyDown(key); },
    [](const Keyboard::KeyboardStateTracker& keyboard, Keyboard::Keys key) { return keyboard.IsKeyReleased(key); },
    [](const Keyboard::KeyboardStateTracker& keyboard, Keyboard::Keys key) { return keyboard.IsKeyPressed(key); }
};

/////////////////////////////////////////////////////////////////////////////

//�ӵ��� ���� �Լ� �����ͷ� ������.
using MouseButtonStateChecker = bool(*)(const Mouse::ButtonStateTracker&, Mouse::ButtonStateTracker::ButtonState);
static bool CheckMouseLeftButton(const Mouse::ButtonStateTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.leftButton == state;
}

static bool CheckMouseRightButton(const Mouse::ButtonStateTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.rightButton == state;
}

static bool CheckMouseMiddleButton(const Mouse::ButtonStateTracker& tracker, Mouse::ButtonStateTracker::ButtonState state) noexcept {
    return tracker.middleButton == state;
}

static inline Mouse::ButtonStateTracker::ButtonState GetMouseKeyState(InputState inputState)
{
    //�� enum�� ���� �����ϴ�.
    return static_cast<Mouse::ButtonStateTracker::ButtonState>(inputState);
}

static bool IsMouseButtonState(
    const Mouse::ButtonStateTracker& mouseTracker, 
    MouseButton mouseButton, 
    Mouse::ButtonStateTracker::ButtonState state) noexcept
{
    constexpr MouseButtonStateChecker mouseButtonCheckers[] = {
        &CheckMouseLeftButton,
        &CheckMouseRightButton,
        &CheckMouseMiddleButton
    };

    return mouseButtonCheckers[static_cast<size_t>(mouseButton)](mouseTracker, state);
}

/////////////////////////////////////////////////////////////////////////////

//Input::GetM() �� Mouse g_mouse; �� �ٲٴ��� �ؾ��Ѵ�. ������ �׽�Ʈ �̱� ������ Input���� �����´�.
void InputManager::Initialize(HWND hwnd)
{
    Input::GetM().SetWindow(hwnd);
}

void InputManager::SetMouseStartOffset(const XMINT2& offset) noexcept
{
    m_startOffset = offset;
}

void InputManager::Update() noexcept
{
    m_keyboardTracker.Update(Input::GetK().GetState());
    DirectX::Mouse::State offset(Input::GetM().GetState());
    offset.x -= m_startOffset.x;
    offset.y -= m_startOffset.y;
    SetPosition({ offset.x, offset.y });
    m_mouseTracker.Update(offset);
}

MouseState InputManager::GetMouseState() const noexcept
{
    auto dxState = Input::GetM().GetState();
    return { m_position, dxState.leftButton };
}

const XMINT2& InputManager::GetPosition() const noexcept
{
    return m_position;
}

bool InputManager::IsInputAction(Keyboard::Keys key, InputState inputState) noexcept
{
    return KeyboardActions[static_cast<size_t>(inputState)](m_keyboardTracker, key);
}

bool InputManager::IsInputAction(Keyboard::Keys key, MouseButton mouseButton) noexcept
{
    //�� Ű�� hold �ϰ� �ڿ��� pressed�� �ؾ� �Ѵ�. �Ѵ� pressed�� �ϸ� ���ÿ� ������ �ϱ� ������ Ű�� �� ������.
    return IsInputAction(key, InputState::Held) && IsInputAction(mouseButton, InputState::Pressed);
}

bool InputManager::IsInputAction(Keyboard::Keys firstKey, Keyboard::Keys secondKey) noexcept
{
    bool firstKeyHeld = IsInputAction(firstKey, InputState::Held);
    bool secondKeyPressed = IsInputAction(secondKey, InputState::Pressed);

    return firstKeyHeld && secondKeyPressed;
    //������ ������ �ʹ� ���� �̺�Ʈ�� ����. �׷��� �ð� ������ �� �����̾��µ� ���������� ������ ��
    //���� �ΰ��� �ð������� ��� �ѹ��� �����Ⱑ ��ư� �ȴ�. �ϴ��� �ϳ��� �ϴ� �ɷ� �ϰ� ���Ŀ� ��
    //�κ��� �����ؼ� ��� Ű�� �������� ���ݰ�, ������ �� ������ ������ �� ������ ������ �����ϰԲ� ��������.
}

bool InputManager::IsInputAction(MouseButton mouseButton, InputState inputState) noexcept
{
    return IsMouseButtonState(m_mouseTracker, mouseButton, GetMouseKeyState(inputState));
}

void InputManager::ResetMouseWheelValue() noexcept
{
    m_lastMouseWheelValue = m_mouseTracker.GetLastState().scrollWheelValue;
}

int InputManager::GetMouseWheelValue() noexcept
{
    const int& curWheelValue = m_mouseTracker.GetLastState().scrollWheelValue;
    int delta = (curWheelValue - m_lastMouseWheelValue) / 120;
    m_lastMouseWheelValue = curWheelValue;

    return delta;
}

/////////////////////////////////////////////////////////////////////////////

unique_ptr<IInputManager> CreateInputManager(HWND hwnd)
{
    auto input = make_unique<InputManager>();
    input->Initialize(hwnd);
    return input;
}