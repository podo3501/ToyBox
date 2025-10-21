#pragma once
#include "../UIFixture.h"

class MockComponent;
enum class ChangeExpect
{
	Unchanged = 0,
	ParentLeftTopChanged = 1 << 0,
	ParentSizeChanged = 1 << 1,
	ParentRelativeChanged = 1 << 2,
	ChildRelativeChanged = 1 << 3,
};

inline ChangeExpect operator|(ChangeExpect a, ChangeExpect b) noexcept
{
	return static_cast<ChangeExpect>(static_cast<int>(a) | static_cast<int>(b));
}

class UIComponentT : public UIFixture
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

	void VerifyTransformChange(function<void()> action, ChangeExpect expect) noexcept;

	MockComponent* m_parent{ nullptr };
	MockComponent* m_child{ nullptr };
};
