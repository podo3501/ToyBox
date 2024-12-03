#include "pch.h"
#include "HelperClass.h"
#include "Utility.h"

void MouseTracker::SetOffset(XMUINT2 offset) noexcept
{
    m_offset = offset;
}

XMUINT2 MouseTracker::GetOffsetPosition() const noexcept
{
    const auto& state = GetLastState();
    return { state.x - m_offset.x, state.y - m_offset.y };
}