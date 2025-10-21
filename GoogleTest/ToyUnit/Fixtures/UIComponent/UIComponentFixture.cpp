#include "pch.h"
#include "UIComponentFixture.h"
#include "Mocks/MockUtils.h"
#include "Mocks/MockComponents.h"

void UIComponentT::SetUp()
{
	UIFixture::SetUp();

	tie(m_parent, m_child) = CreateTwoLevelComponents<MockComponent>(m_main);
}

void UIComponentT::TearDown()
{
	UIFixture::TearDown();
}

static inline bool Has(ChangeExpect v, ChangeExpect f) noexcept
{
    return (static_cast<int>(v) & static_cast<int>(f)) != 0;
}

void UIComponentT::VerifyTransformChange(function<void()> action, ChangeExpect expect) noexcept
{
    const XMINT2 preParentLeftTop = m_parent->GetLeftTop();
    const XMUINT2 preParentSize = m_parent->GetSize();
    const XMINT2 preParentRel = m_parent->GetRelativePosition();
    const XMINT2 preChildRel = m_child->GetRelativePosition();

    ExecuteAndUpdate(m_main, [this, &action] { action(); });

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