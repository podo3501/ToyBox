#include "pch.h"
#include "InputManager.h"
#include "HelperClass.h"

InputManager::~InputManager() = default;
InputManager::InputManager(HWND hwnd) :
    m_keyboard{ make_unique<Keyboard>() },
    m_keyboardTracker{ make_unique<Keyboard::KeyboardStateTracker>() },
    m_mouse{ make_unique<Mouse>() },
    m_mouseTracker{ make_unique<MouseTracker>() }
{
    m_mouse->SetWindow(hwnd);
}

void InputManager::Update()
{
    m_keyboardTracker->Update(m_keyboard->GetState());
    m_mouseTracker->Update(m_mouse->GetState());
}

void InputManager::Update(const Mouse::State& state)
{
    m_mouseTracker->Update(state);
}

