#pragma once

class MouseTracker : public Mouse::ButtonStateTracker
{
public:
    void SetOffset(XMINT2 offset) noexcept;
    XMINT2 GetOffsetPosition() const noexcept;

private:
    XMINT2 m_offset{};
};