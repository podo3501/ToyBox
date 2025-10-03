#include "pch.h"
#include "Input.h"

XMINT2 MouseTracker::GetPosition() const noexcept
{
    return { GetLastState().x, GetLastState().y };
}

bool __cdecl KeyboardTracker::IsKeyHeld(Keyboard::Keys key) const noexcept
{
    return GetLastState().IsKeyDown(key);
}

///////////////////////////////////////////////////////

KeyboardTracker Input::m_keyboardTracker;
Keyboard Input::m_keyboard;

MouseTracker Input::m_mouseTracker;
Mouse Input::m_mouse;

optional<XMINT2> Input::m_startOffset = nullopt;

void Input::Initialize(HWND hwnd)
{
    m_mouse.SetWindow(hwnd);
}

void Input::Update() noexcept
{
    m_keyboardTracker.Update(m_keyboard.GetState());
    const auto& state = m_mouse.GetState();
    if (m_startOffset)
    {
        Mouse::State offsetState(state);
        offsetState.x -= m_startOffset->x;
        offsetState.y -= m_startOffset->y;
        m_mouseTracker.Update(offsetState);
        return;
    }

    m_mouseTracker.Update(state);
}