#pragma once

class MouseTracker : public Mouse::ButtonStateTracker
{
public:
    void SetOffset(XMUINT2 offset) noexcept;
    XMUINT2 GetOffsetPosition() const noexcept;

private:
    XMUINT2 m_offset{};
};