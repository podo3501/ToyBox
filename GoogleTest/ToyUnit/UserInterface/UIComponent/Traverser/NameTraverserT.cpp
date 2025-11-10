#include "pch.h"
#include "NameTraverserT.h"
#include "Shared/Utils/StlExt.h"

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(NameTraverserT, FindComponent) //자신의 Region에서만 찾는것
	{
		RenameRegion(m_main, "MainRegion");

		//auto find = UIEx(m_main).FindComponent("Unknown");
		//EXPECT_EQ(m_main, find);
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
		auto [owner, component] = GetPtrs(CreateComponent<MockComponent>());
		AttachComponent(m_main, move(owner));

		EXPECT_TRUE(RenameRegion(component, "ComponentRegion"));
		EXPECT_EQ(component->GetName(), "Unknown");

		EXPECT_TRUE(RenameRegion(component, ""));
		EXPECT_EQ(component->GetName(), "Unknown_1");
	}

	TEST_F(NameTraverserT, RenameRegion_MergeRegion2)
	{
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
		auto nameGen = UIManager::GetNameGenerator();

		RenameRegion(component, "");
		EXPECT_TRUE(nameGen->IsUnusedRegion("ComponentRegion"));

		RenameRegion(m_main, "");
		EXPECT_TRUE(nameGen->IsUnusedRegion("MainRegion"));
	}
}

//locator + namespace + 일반 클래스 조합으로 해서 
//UIEx(m_main).AttachComponent(move(parentOwner));
//이런 형태가 아니라 UITraverser::AttachComponent(m_main, move(parentOwner));
//이렇게 한다. 그리고 using을 사용하면 UITraverser를 안써도 되니까 cpp 쪽 코드에 지저분함이
//없어지겠지? 단점은 만들고 나면 namespace에서 연결을 해 줘야 하는 정도일듯.
