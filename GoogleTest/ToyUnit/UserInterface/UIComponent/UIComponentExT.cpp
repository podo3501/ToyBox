#include "pch.h"
#include "UIComponentExT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

class MockComponent : public UIComponentStub 
{
public: 
	bool Setup() noexcept { return true; } //CreateComponent �Ҷ� �ʿ��� �Լ�.
};

namespace UserInterfaceT
{
	TEST_F(UIComponentExT, AttachComponent)
	{
		auto component = CreateComponent<MockComponent>();
		EXPECT_EQ(UIEx(m_main).AttachComponent(move(component)), nullptr); //attach�� �� �Ǹ� nullptr�� ����, �߸��Ǹ� move�� component�� �����Ѵ�.
	}

	TEST_F(UIComponentExT, AttachComponent_FindComponent)
	{
		//UIEx(m_main)�� region�� name�� ���ڷ� ������ ã�Ƽ� attach �ϴ� ���� ������ �Ѵ�.
		//����� ���� ������ ���Ŀ� �ۼ� ���
	}

	TEST_F(UIComponentExT, DetachComponent)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		UIEx(m_main).AttachComponent(move(owner));

		auto[detach, parent] = UIEx(component).DetachComponent();
		EXPECT_TRUE(*detach == *component);
		EXPECT_TRUE(*parent == *m_main); //��� ���������� �θ� ������Ʈ�� �˷��ش�.
	}

	TEST_F(UIComponentExT, DetachComponent_FindComponent)
	{
		//����� ���� ������ ���Ŀ� �ۼ� ���
	}

	TEST_F(UIComponentExT, Rename)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>()); 
		UIEx(m_main).AttachComponent(move(owner)); // attach �Ҷ� �̸��� �����. name: Unknown

		EXPECT_TRUE(UIEx(component).Rename("NewName"));
		//?!?���� root region �� root name�� nameGenerator�� ����� �ȵ� �ִ� ���װ� �ִ�.
		//EXPECT_FALSE(UIEx(component).Rename("Main")); �̰��� ��� �Ǿ�� �Ѵ�.
	}
}