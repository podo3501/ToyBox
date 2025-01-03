#pragma once

class MouseTracker : public Mouse::ButtonStateTracker
{
public:
    XMINT2 GetOffsetPosition() const noexcept;

    //������ �������� window�� �ֱ� ������ ���콺�� �� �����쿡 ���ؼ� ��ǥ�� ������ �Ѵ�.
    void PushOffset(const ImVec2& offset) noexcept;
    void PushOffset(const XMINT2& offset) noexcept;

    void PopOffset() noexcept;  //Push�� ���� ���缭 �ڵ��Ѵ�.

private:
    XMINT2 m_offset{};
    stack<XMINT2> m_stackOffset;
};