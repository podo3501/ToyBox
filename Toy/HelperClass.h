#pragma once

class MouseTracker : public Mouse::ButtonStateTracker
{
public:
    XMINT2 GetOffsetPosition() const noexcept;

    //툴에는 여러개의 window가 있기 때문에 마우스가 각 윈도우에 대해서 좌표를 가져야 한다.
    void PushOffset(const ImVec2& offset) noexcept;
    void PushOffset(const XMINT2& offset) noexcept;

    void PopOffset() noexcept;  //Push와 쌍을 맞춰서 코딩한다.

private:
    XMINT2 m_offset{};
    stack<XMINT2> m_stackOffset;
};