#include "pch.h"
#include "UIComponentExT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/Input/IMouseEventReceiver.h"

class MockComponent : public UIComponentStub 
{
public: 
	bool Setup() noexcept { return true; } //CreateComponent 할때 필요한 함수.
};

class MockReceiverComponent : public UIComponentStub, public IMouseEventReceiver
{
public:
	virtual IMouseEventReceiver* AsMouseEventReceiver() noexcept override { return this; }
	bool Setup() noexcept { return true; } //CreateComponent 할때 필요한 함수.
};

namespace UserInterfaceT::UIComponentT
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

	TEST_F(UIComponentExT, PickComponents)
	{
		UILayout childLayout{ {100, 100}, Origin::LeftTop };
		UILayout parentLayout{ {50, 50}, Origin::LeftTop };
		auto [owner, child] = CreateMockComponent<MockComponent>(childLayout);
		auto parent = CreateComponent<RenderTexture>(parentLayout, move(owner));
		parent->UpdatePositionsManually();

		EXPECT_EQ(UIEx(parent).PickComponents({ 45, 45 }).size(), 2); //RenderTexture 안쪽이니까 2개가 있어야 한다.
		EXPECT_EQ(UIEx(parent).PickComponents({ 65, 65 }).size(), 0); //RenderTexture 바깥이니까 아무것도 없어야 한다.
	}

	TEST_F(UIComponentExT, PickMouseReceivers)
	{
		UILayout childLayout{ {100, 100}, Origin::LeftTop };
		UILayout parentLayout{ {50, 50}, Origin::LeftTop };
		auto [owner, child] = CreateMockComponent<MockReceiverComponent>(childLayout);
		auto parent = CreateComponent<RenderTexture>(parentLayout, move(owner));
		parent->UpdatePositionsManually();

		EXPECT_EQ(UIEx(parent).PickMouseReceivers({ 45, 45 }).size(), 1); //RenderTexture 안쪽이니까 1개가 있어야 한다.
		EXPECT_EQ(UIEx(parent).PickMouseReceivers({ 65, 65 }).size(), 0); //RenderTexture 바깥이니까 아무것도 없어야 한다.
	}

	TEST_F(UIComponentExT, Rename)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>()); 
		UIEx(m_main).AttachComponent(move(owner)); // attach 할때 이름이 생긴다. name: Unknown

		EXPECT_TRUE(UIEx(component).Rename("NewName"));
		EXPECT_FALSE(UIEx(component).Rename("Main"));
	}

	TEST_F(UIComponentExT, RenameRegion_EmptyB)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		UIEx(m_main).AttachComponent(move(owner)); // attach 할때 이름이 생긴다. name: Unknown

		UIEx(component).RenameRegion("NewRegion");
		UIEx(component).RenameRegion("");
	}

}