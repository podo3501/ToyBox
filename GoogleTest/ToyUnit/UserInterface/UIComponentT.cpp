#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"

class MockComponent : public UIComponentStub
{};

namespace UserInterface
{
	TEST_F(UIComponentT, LockedSize)
	{
		XMUINT2 oldSize{ 64, 64 }, newSize{ 128, 128 };
		UILayout layout{ oldSize, Origin::Center };
		auto [comp, compPtr] = CreateMockComponent<MockComponent>(layout);
		UIEx(m_main).AttachComponent(move(comp), { 400, 300 });

		compPtr->SetStateFlag(StateFlag::X_SizeLocked, true);
		compPtr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		compPtr->ChangeSize(newSize);

		EXPECT_EQ(compPtr->GetSize(), oldSize);
	}
}