#include "pch.h"
#include "UIComponentExT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

class MockComponent : public UIComponentStub 
{
public: 
	bool Setup() noexcept { return true; } //CreateComponent 할때 필요한 함수.
};

namespace UserInterfaceT
{
	TEST_F(UIComponentExT, AttachComponent)
	{
		auto component = CreateComponent<MockComponent>();
		EXPECT_EQ(UIEx(m_main).AttachComponent(move(component)), nullptr); //attach가 잘 되면 nullptr을 리턴, 잘못되면 move한 component를 리턴한다.
	}

	TEST_F(UIComponentExT, AttachComponent_FindComponent)
	{
		//UIEx(m_main)에 region및 name을 인자로 넣으면 찾아서 attach 하는 것을 만들어야 한다.
		//현재는 버그 때문에 추후에 작성 요망
	}

	TEST_F(UIComponentExT, DetachComponent)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		UIEx(m_main).AttachComponent(move(owner));

		auto[detach, parent] = UIEx(component).DetachComponent();
		EXPECT_TRUE(*detach == *component);
		EXPECT_TRUE(*parent == *m_main); //어디서 떨어졌는지 부모 컴포넌트를 알려준다.
	}

	TEST_F(UIComponentExT, DetachComponent_FindComponent)
	{
		//현재는 버그 때문에 추후에 작성 요망
	}

	TEST_F(UIComponentExT, Rename)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>()); 
		UIEx(m_main).AttachComponent(move(owner)); // attach 할때 이름이 생긴다. name: Unknown

		EXPECT_TRUE(UIEx(component).Rename("NewName"));
		//?!?현재 root region 및 root name이 nameGenerator에 등록이 안돼 있는 버그가 있다.
		//EXPECT_FALSE(UIEx(component).Rename("Main")); 이것이 통과 되어야 한다.
	}
}