#pragma once
#include "MockComponent.h"
#include "ComponentHelper.h"

enum class ChangeExpect
{
	Unchanged = 0, //아무것도 바뀌지 않았다면
	ParentLeftTopChanged = 1 << 0, //부모 LeftTop 위치가 바뀌었는지
	ParentSizeChanged = 1 << 1, //부모 Size가 바뀌었는지
	ParentRelativeChanged = 1 << 2, //부모 상대적 좌표가 바뀌었는지
	ChildRelativeChanged = 1 << 3, //자식 상대적 좌표가 바뀌었는지
};

inline ChangeExpect operator|(ChangeExpect a, ChangeExpect b) noexcept
{
	return static_cast<ChangeExpect>(static_cast<int>(a) | static_cast<int>(b));
}

class UIComponentT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
    virtual void TearDown() override {};
    void VerifyTransformChange(function<void()> action, ChangeExpect expect) noexcept;

    unique_ptr<MockComponent> m_main{ nullptr };
    MockComponent* m_parent{ nullptr };
    MockComponent* m_child{ nullptr };

private:
    bool Has(ChangeExpect v, ChangeExpect f) noexcept;
};

///////////////////////////////////////////////////////////////

void UIComponentT::SetUp()
{
    UILayout layout{ {800, 600}, Origin::LeftTop };
    tie(m_main, std::ignore) = CreateMockComponent<MockComponent>(layout);
    m_parent = AttachMockComponent<MockComponent>(m_main.get(), mock_defaults::parentDesc);
    m_child = AttachMockComponent<MockComponent>(m_parent, mock_defaults::childDesc);
}

bool UIComponentT::Has(ChangeExpect v, ChangeExpect f) noexcept
{
    return (static_cast<int>(v) & static_cast<int>(f)) != 0;
}
//이전의 상태와 action 한 이후에 어떤게 바뀌었는지 확인. ChangeExpect enum 값으로 확인
void UIComponentT::VerifyTransformChange(function<void()> action, ChangeExpect expect) noexcept
{
    const XMINT2 preParentLeftTop = m_parent->GetLeftTop();
    const XMUINT2 preParentSize = m_parent->GetSize();
    const XMINT2 preParentRel = m_parent->GetRelativePosition();
    const XMINT2 preChildRel = m_child->GetRelativePosition();

    ExecuteAndUpdate(m_main.get(), [this, &action] { action(); });

    if (Has(expect, ChangeExpect::ParentLeftTopChanged))
        EXPECT_NE(preParentLeftTop, m_parent->GetLeftTop());
    else
        EXPECT_EQ(preParentLeftTop, m_parent->GetLeftTop());

    if (Has(expect, ChangeExpect::ParentSizeChanged))
        EXPECT_NE(preParentSize, m_parent->GetSize());
    else
        EXPECT_EQ(preParentSize, m_parent->GetSize());

    if (Has(expect, ChangeExpect::ParentRelativeChanged))
        EXPECT_NE(preParentRel, m_parent->GetRelativePosition());
    else
        EXPECT_EQ(preParentRel, m_parent->GetRelativePosition());

    if (Has(expect, ChangeExpect::ChildRelativeChanged))
        EXPECT_NE(preChildRel, m_child->GetRelativePosition());
    else
        EXPECT_EQ(preChildRel, m_child->GetRelativePosition());
}
