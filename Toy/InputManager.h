#pragma once

class MouseTracker;

namespace DirectX
{
    class Keyboard::KeyboardStateTracker;
}

class InputManager
{
public:
    InputManager(HWND hwnd);
    ~InputManager();

    void Update();
    void Update(const Mouse::State& state);

    inline DirectX::Keyboard::KeyboardStateTracker* GetKeyboard() noexcept { return m_keyboardTracker.get(); }
    inline MouseTracker* GetMouse() noexcept { return m_mouseTracker.get(); }

private:
    unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
    unique_ptr<DirectX::Keyboard> m_keyboard;

    unique_ptr<MouseTracker> m_mouseTracker;
    unique_ptr<Mouse> m_mouse;
};
