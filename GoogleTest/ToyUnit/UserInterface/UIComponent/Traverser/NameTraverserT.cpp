#include "pch.h"
#include "NameTraverserT.h"
#include "../MockComponent.h"
#include "Shared/Utils/StlExt.h"

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(NameTraverserT, AttachComponent)
	{
		auto component = CreateComponent<MockComponent>();
		EXPECT_EQ(AttachComponent(m_main, move(component)), nullptr); //attach가 잘 되면 nullptr을 리턴, 잘못되면 move한 component를 리턴한다.
	}

	TEST_F(NameTraverserT, AttachComponent_CloneRegion)
	{
		auto [owner, child] = CreateMockComponent<MockComponent>();
		AttachComponent(m_main, move(owner));
		RenameRegion(child, "Region");

		auto [cloneOwner, clone] = GetPtrs(Clone(child));
		AttachComponent(m_main, move(cloneOwner));

		EXPECT_EQ(clone->GetRegion(), "Region_1"); //Region 컴포넌트를 Clone 하면 Region에 _1 이 붙어서 생성된다.
	}

	TEST_F(NameTraverserT, DetachComponent)
	{
		auto [owner, component] = CreateMockComponent<MockComponent>();
		AttachComponent(m_main, move(owner));

		auto [detach, parent] = DetachComponent(component);
		EXPECT_TRUE(*detach == *component);
		EXPECT_TRUE(*parent == *m_main); //어디서 떨어졌는지 부모 컴포넌트를 알려준다.
	}

	TEST_F(NameTraverserT, DetachComponent_CheckRegion)
	{
		auto component = AttachMockComponent<MockComponent>(m_main);
		component->SetRegion("Region");

		DetachComponent(component);

		auto nameGen = m_uiModule->GetNameGenerator();
		EXPECT_TRUE(nameGen->IsUnusedRegion("Region")); //지우고 나서 사용할 수 있는지 확인
	}

	TEST_F(NameTraverserT, FindComponent) //자신의 Region에서만 찾는것
	{
		auto component = AttachMockComponent<MockComponent>(m_main);
		RenameRegion(component, "MainRegion");

		EXPECT_EQ(FindComponent(m_main, "Main"), m_main);
		EXPECT_EQ(FindComponent(component, "Unknown"), component);
	}

	TEST_F(NameTraverserT, FindRegionComponent) //모든 노드를 돌면서 Region을 찾는것
	{
		RenameRegion(m_main, "MainRegion");
		EXPECT_EQ(FindRegionComponent(m_main, "MainRegion"), m_main);
	}

	TEST_F(NameTraverserT, FindRegionComponent_EmptyRegion)
	{
		RenameRegion(m_main, "");
		auto component = AttachMockComponent<MockComponent>(m_main);

		auto find = FindRegionComponent(component, "");
		EXPECT_EQ(m_main, find);
	}

	TEST_F(NameTraverserT, Rename)
	{
		auto [owner, component] = CreateMockComponent<MockComponent>();
		AttachComponent(m_main, move(owner));
		
		EXPECT_TRUE(Rename(m_main, "NewName"));
		EXPECT_FALSE(Rename(component, "NewName")); //unique한 이름만 되기 때문에 실패.
	}

	TEST_F(NameTraverserT, RenameRegion)
	{
		EXPECT_TRUE(RenameRegion(m_main, "region"));
	}

	TEST_F(NameTraverserT, RenameRegion_ChildName)
	{
		auto [parentOwner, parent] = GetPtrs(CreateComponent<MockComponent>());
		auto [childOwner, child] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(parentOwner)); //parent name: Unknown_1
		AttachComponent(parent, move(childOwner)); //child name: Unknown_2
		RenameRegion(parent, "NewRegion");

		EXPECT_EQ(parent->GetName(), "Unknown"); //parent가 NewRegion이 되면서 Unknown으로 이름이 바뀐다. 
		EXPECT_EQ(child->GetName(), "Unknown_1"); //그래서 자식은 Panel_1로 바뀐다.
	}

	//Region 통합 테스트는 상위 region이 "" 일때와 값이 있을때 2가지만 확인하면 된다.
	TEST_F(NameTraverserT, RenameRegion_MergeRegion1)
	{
		Rename(m_main, "Unknown");
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(owner));

		EXPECT_TRUE(RenameRegion(component, "ComponentRegion"));
		EXPECT_EQ(component->GetName(), "Unknown");

		EXPECT_TRUE(RenameRegion(component, ""));
		EXPECT_EQ(component->GetName(), "Unknown_1");
	}

	TEST_F(NameTraverserT, RenameRegion_MergeRegion2)
	{
		Rename(m_main, "Unknown");
		RenameRegion(m_main, "MainRegion");
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(owner));

		EXPECT_TRUE(RenameRegion(component, "ComponentRegion"));
		EXPECT_EQ(component->GetName(), "Unknown");

		EXPECT_TRUE(RenameRegion(component, ""));
		EXPECT_EQ(component->GetName(), "Unknown_1");
	}

	TEST_F(NameTraverserT, RenameRegion_Remove)
	{
		RenameRegion(m_main, "MainRegion");
		RenameRegion(m_main, "");

		EXPECT_EQ(m_main->GetRegion(), "");
	}

	TEST_F(NameTraverserT, RenameRegion_SameRegion)
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(owner));
		RenameRegion(m_main, "MainRegion");

		EXPECT_FALSE(RenameRegion(component, "MainRegion"));
	}

	TEST_F(NameTraverserT, RenameRegion_SyncNameGen) //Region Merge후 이름생성기와 실제 노드간 이름이 동기화 됐는지 체크
	{
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(owner));
		RenameRegion(m_main, "MainRegion");
		RenameRegion(component, "ComponentRegion");
		auto nameGen = m_uiModule->GetNameGenerator();

		RenameRegion(component, "");
		EXPECT_TRUE(nameGen->IsUnusedRegion("ComponentRegion"));

		RenameRegion(m_main, "");
		EXPECT_TRUE(nameGen->IsUnusedRegion("MainRegion"));
	}
}
