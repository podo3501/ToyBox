#include "pch.h"
#include "HelperClass.h"

void CustomButtonStateTracker::SetOffset(XMUINT2 offset) noexcept
{
    m_offset = offset;
}

XMUINT2 CustomButtonStateTracker::GetOffsetPosition() const noexcept
{
    const auto& state = GetLastState();
    return { state.x - m_offset.x, state.y - m_offset.y };
}

