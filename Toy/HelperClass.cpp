#include "pch.h"
#include "HelperClass.h"
#include "Utility.h"

void MouseTracker::PushOffset(const ImVec2& offset) noexcept
{
    PushOffset(ImVec2ToXMINT2(offset));
}

void MouseTracker::PushOffset(const XMINT2& offset) noexcept
{
    m_stackOffset.push(offset);
}

void MouseTracker::PopOffset() noexcept
{
    m_stackOffset.pop();
}

XMINT2 MouseTracker::GetOffsetPosition() const noexcept
{
    const auto& offset = m_stackOffset.top();
    const auto& state = GetLastState();
    return { state.x - offset.x, state.y - offset.y };
}

///////////////////////////////////////////////////////////

bool __cdecl KeyboardTracker::IsKeyHeld(Keyboard::Keys key) const noexcept 
{ 
    return GetLastState().IsKeyDown(key); 
}