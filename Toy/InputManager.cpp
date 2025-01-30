#include "pch.h"
#include "InputManager.h"

XMINT2 MouseTracker::GetPosition() const noexcept
{
    return { GetLastState().x, GetLastState().y };
}

bool __cdecl KeyboardTracker::IsKeyHeld(Keyboard::Keys key) const noexcept
{
    return GetLastState().IsKeyDown(key);
}

///////////////////////////////////////////////////////

KeyboardTracker InputManager::m_keyboardTracker;
Keyboard InputManager::m_keyboard;

MouseTracker InputManager::m_mouseTracker;
Mouse InputManager::m_mouse;

void InputManager::Initialize(HWND hwnd)
{
    m_mouse.SetWindow(hwnd);
}

void InputManager::Update() noexcept
{
    m_keyboardTracker.Update(m_keyboard.GetState());
    m_mouseTracker.Update(m_mouse.GetState());
}