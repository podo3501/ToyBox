#include "pch.h"
#include "UIComponentExT.h"
#include "MockComponent.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/Input/IMouseEventReceiver.h"
#include "Toy/UserInterface/UINameGenerator/UINameGenerator.h"

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
		UIEx(m_main).RenameRegion("MainRegion");
		auto child = AttachMockComponent<MockComponent>(m_main);
		UIEx(child).Rename("Child");
		
		auto component = CreateComponent<MockComponent>();
		EXPECT_EQ(UIEx(m_main).AttachComponent("MainRegion", "Child", move(component)), nullptr);
	}

	TEST_F(UIComponentExT, AttachComponent_CloneRegion)
	{
		auto [owner, child] = GetPtrs(CreateComponent<MockComponent>());
		UIEx(m_main).AttachComponent(move(owner));
		UIEx(child).RenameRegion("Region");
		
		auto [cloneOwner, clone] = GetPtrs(child->Clone());
		UIEx(m_main).AttachComponent(move(cloneOwner));

		EXPECT_EQ(clone->GetRegion(), "Region_1"); //Region 컴포넌트를 Clone 하면 Region에 _1 이 붙어서 생성된다.
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
		UIEx(m_main).RenameRegion("MainRegion");
		auto component = AttachMockComponent<MockComponent>(m_main);
		UIEx(component).Rename("Component");
		
		auto [detach, parent] = UIEx(component).DetachComponent("MainRegion", "Component");// 꼭 component가 아니더라도 어느 컴포넌트에서 시작해도 Detach 할 수 있다.
		EXPECT_TRUE(*detach == *component);
		EXPECT_TRUE(*parent == *m_main);
	}

	TEST_F(UIComponentExT, DetachComponent_CheckRegion)
	{
		auto component = AttachMockComponent<MockComponent>(m_main);
		UIEx(component).RenameRegion("Region");
		
		UIEx(component).DetachComponent();
		auto nameGen = m_uiModule->GetNameGenerator();
		EXPECT_TRUE(nameGen->IsUnusedRegion("Region")); //지우고 나서 사용할 수 있는지 확인
	}

	TEST_F(UIComponentExT, FindComponent) //자신의 Region에서만 찾는것
	{
		UIEx(m_main).Rename("Panel");
		UIEx(m_main).RenameRegion("MainRegion");

		auto find = UIEx(m_main).FindComponent("Panel");
		EXPECT_EQ(m_main, find);
	}

	TEST_F(UIComponentExT, FindRegionComponent) //모든 노드를 돌면서 Region을 찾는것
	{
		UIEx(m_main).Rename("Panel");
		UIEx(m_main).RenameRegion("MainRegion");

		auto find = UIEx(m_main).FindRegionComponent("MainRegion");
		EXPECT_EQ(m_main, find);
	}

	TEST_F(UIComponentExT, FindRegionComponent_EmptyRegion)
	{
		UIEx(m_main).RenameRegion("");
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		UIEx(m_main).AttachComponent(move(owner));

		auto find = UIEx(component).FindRegionComponent("");
		EXPECT_EQ(m_main, find);
	}

	TEST_F(UIComponentExT, GetChildrenBoundsSize)
	{
		m_main->ChangeSize(10, 10);
		ComponentDesc desc(XMINT2{ 5, 5 }, UILayout({ 10, 10 })); //크기는 10, 10, 위치는 5, 5 에 attach 한다.
		auto component = AttachMockComponent<MockComponent>(m_main, desc);

		EXPECT_EQ(UIEx(m_main).GetChildrenBoundsSize(), XMUINT2(15, 15));
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

	TEST_F(UIComponentExT, RenameRegion)
	{
		EXPECT_TRUE(UIEx(m_main).RenameRegion("NewRegion"));
		EXPECT_EQ(m_main->GetRegion(), "NewRegion");
	}

	TEST_F(UIComponentExT, RenameRegion_ChildName)
	{
		UIEx(m_main).Rename("Panel");
		auto [parentOwner, parent] = GetPtrs(CreateComponent<Panel>()); //Panel_1
		auto [childOwner, child] = GetPtrs(CreateComponent<Panel>()); //Panel_2
		UIEx(parent).AttachComponent(move(childOwner));
		UIEx(m_main).AttachComponent(move(parentOwner));
		EXPECT_TRUE(UIEx(parent).RenameRegion("NewRegion"));

		EXPECT_EQ(parent->GetName(), "Panel"); //parent가 NewRegion이 되면서 Panel로 이름이 바뀐다. 
		EXPECT_EQ(child->GetName(), "Panel_1"); //그래서 자식은 Panel_1로 바뀐다.
	}

	//RemaneRegion이 윗 Region과 통합되는 상태는 2가지이다. 윗 Region이 "" 일때와 Region 값이 있을때이다.
	TEST_F(UIComponentExT, RenameRegion_MergeRegion1)
	{
		UIEx(m_main).Rename("Panel");
		auto [owner, component] = GetPtrs(CreateComponent<Panel>());
		UIEx(m_main).AttachComponent(move(owner));

		EXPECT_TRUE(UIEx(component).RenameRegion("ComponentRegion"));
		EXPECT_EQ(component->GetName(), "Panel");

		EXPECT_TRUE(UIEx(component).RenameRegion(""));
		EXPECT_EQ(component->GetName(), "Panel_1");
	}

	TEST_F(UIComponentExT, RenameRegion_MergeRegion2)
	{
		UIEx(m_main).Rename("Panel");
		auto [owner, component] = GetPtrs(CreateComponent<Panel>());
		UIEx(m_main).AttachComponent(move(owner));
		UIEx(m_main).RenameRegion("MainRegion");

		EXPECT_TRUE(UIEx(component).RenameRegion("ComponentRegion"));
		EXPECT_EQ(component->GetName(), "Panel");

		EXPECT_TRUE(UIEx(component).RenameRegion(""));
		EXPECT_EQ(component->GetName(), "Panel_1");
	}

	TEST_F(UIComponentExT, RenameRegion_Remove)
	{
		UIEx(m_main).RenameRegion("MainRegion");
		EXPECT_TRUE(UIEx(m_main).RenameRegion(""));

		EXPECT_EQ(m_main->GetRegion(), "");
	}

	TEST_F(UIComponentExT, RenameRegion_SameRegion)
	{
		UIEx(m_main).Rename("Panel");
		auto [owner, component] = GetPtrs(CreateComponent<Panel>());
		UIEx(m_main).AttachComponent(move(owner));
		EXPECT_TRUE(UIEx(m_main).RenameRegion("MainRegion"));
		
		EXPECT_FALSE(UIEx(component).RenameRegion("MainRegion"));
	}

	TEST_F(UIComponentExT, RenameRegion_SyncNameGen) //Region Merge후 이름생성기와 실제 노드간 이름이 동기화 됐는지 체크
	{
		UIEx(m_main).Rename("Panel");
		auto [owner, component] = GetPtrs(CreateComponent<Panel>());
		UIEx(m_main).AttachComponent(move(owner));
		UIEx(m_main).RenameRegion("MainRegion");
		UIEx(component).RenameRegion("ComponentRegion");
		auto nameGen = m_uiModule->GetNameGenerator();

		UIEx(component).RenameRegion("");
		EXPECT_TRUE(nameGen->IsUnusedRegion("ComponentRegion"));

		UIEx(m_main).RenameRegion("");
		EXPECT_TRUE(nameGen->IsUnusedRegion("MainRegion"));
	}

	//?!? 패널을 써서 붙이니까 아무래도 의존성있는 테스트가 될 수밖에 없다.
	//UIModule이 Locator로 빠지면 패널이 main이 아니더라도 가능하니까 그때 되면 panel말고도 mock으로 만들어서 테스트 가능할 것이다.
}