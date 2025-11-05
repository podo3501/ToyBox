#include "pch.h"
#include "Fixtures/FixtureSuite.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "Toy/UserInterface/UINameGenerator/UINameGenerator.h"
#include "Toy/UserInterface/UIModule.h"
#include "Shared/Utils/StlExt.h"

namespace UserInterface
{
	//툴에서 임시로 component를 생성할때 module이 생성되지 않은 상태이기 때문에 테스트가 필요
	TEST_F(Integration, AttachDetachNoModuleTest) //옮기는거 완료
	{
		auto [main, mainPtr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));

		EXPECT_TRUE(UIEx(mainPtr).Rename("main"));
		EXPECT_TRUE(UIEx(tex1Ptr).RenameRegion("region"));
		UIEx(main).AttachComponent(move(tex1), { 100, 100 });

		auto result = UIEx(tex1Ptr).DetachComponent();
		EXPECT_NE(result.first.get(), nullptr);
		EXPECT_NE(result.second, nullptr);
	}

	static bool AttachComponentHelper(UIComponent* main, const string& parentName) noexcept
	{
		auto patchTex1 = CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1");
		return UIEx(main).AttachComponent("UIModuleMainEntry", parentName, move(patchTex1), { 10, 10 }) ? false : true;
	}

	static bool DetachComponentHelper(UIComponent* main, const string& componentName) noexcept
	{
		auto [detach, parent] = UIEx(main).DetachComponent("UIModuleMainEntry", componentName);
		return detach != nullptr;
	}

	TEST_F(Integration, AttachDetachTest)
	{
		auto tex9 = CreateComponent<PatchTextureStd9>(UILayout{ {200, 100}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9), { 80, 60 });

		EXPECT_FALSE(AttachComponentHelper(m_main, "PatchTextureStd9")); //Tex9에는 Attach 안됨 
		EXPECT_TRUE(DetachComponentHelper(m_main, "PatchTextureStd9")); //위에서 PatchTextureStd1를 attach 했다.
		EXPECT_TRUE(AttachComponentHelper(m_main, "Main"));
		EXPECT_TRUE(UIEx(m_main).FindComponent("PatchTextureStd1")); //Detach 하고 이름을 반환했다면 PatchTextureStd1 이름이어야 한다.
		EXPECT_TRUE(DetachComponentHelper(m_main, "PatchTextureStd1"));

		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {200, 100}, Origin::LeftTop }, "BackImage1"));
		auto tex2 = CreateComponent<PatchTextureStd1>(UILayout{ {110, 60}, Origin::LeftTop }, "BackImage1");
		UIEx(tex1).AttachComponent(move(tex2), { 100, 50 });	//중점에 attach 한다.
		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });
		m_uiModule->Update(m_timer);

		EXPECT_EQ(UIEx(tex1Ptr).GetChildrenBoundsSize(), XMUINT2(210, 110));
		auto [detached, parent] = UIEx(tex1Ptr).DetachComponent();
		detached->ProcessUpdate(m_timer);
		EXPECT_EQ(UIEx(detached).GetChildrenBoundsSize(), XMUINT2(210, 110));
	}

	TEST_F(Integration, DetachThenNameTest)
	{
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex2, tex2Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(tex1).RenameRegion("Region");
		UIEx(tex1).AttachComponent(move(tex2), { 100, 100 });
		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });
		EXPECT_TRUE(UIEx(tex1Ptr).FindComponent("PatchTextureStd1"));
		EXPECT_TRUE(tex2Ptr->GetRegion().empty());

		UIEx(tex1Ptr).DetachComponent();
		EXPECT_FALSE(UIEx(m_main).FindComponent("PatchTextureStd1"));

		//region이 잘 지워졌는지 테스트
		auto [tex3, tex3Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(tex3).RenameRegion("Region");
		UIEx(m_main).AttachComponent(move(tex3), { 100, 100 });
		EXPECT_EQ(tex3Ptr->GetRegion(), "Region");
		EXPECT_TRUE(UIEx(tex3Ptr).FindComponent("PatchTextureStd1"));
	}

	TEST_F(Integration, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd1>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage1")));
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd3>(UILayout{ { 100, 36 }, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal")));
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd9>(UILayout{ { 220, 190 }, Origin::LeftTop }, "BackImage9")));
		vector<wstring> bindFontKeys{ L"Hangle", L"English" };
		EXPECT_TRUE(VerifyClone(CreateComponent<TextArea>(UILayout{ { 220, 190 }, Origin::LeftTop }, L"<Hangle>테스트 입니다!</Hangle>", bindFontKeys)));
		EXPECT_TRUE(VerifyClone(CreateComponent<TextureSwitcher>(UILayout{ { 220, 190 }, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"))));
		EXPECT_TRUE(VerifyClone(CreateComponent<TextureSwitcher>(UILayout{ { 48, 100 }, Origin::Center }, TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"))));
		EXPECT_TRUE(VerifyClone(CreateSampleListArea({ { 220, 190 }, Origin::LeftTop })));
	}

	static vector<ComponentID> GetComponentIDs(const vector<UIComponent*>& components)
	{
		vector<ComponentID> componentIDs;
		ranges::transform(components, back_inserter(componentIDs), [](auto component) {
			return component->GetTypeID(); });
		return componentIDs;
	}
	//데이터에 특정한 패턴이 있는지 검사한다.
	template<typename T>
	static bool ContainsSequence(const vector<T>& haystack, const vector<T>& needle)
	{
		return !ranges::search(haystack, needle).empty();
	}

	TEST_F(Integration, MatchesRenderOrder) //옮길때 실제로 mock 4-5개 붙여서 순서대로 call 되는지 확인하는걸로 만들어야 한다.
	{
		using enum ComponentID;

		auto listArea = CreateSampleListArea({ { 200, 170 }, Origin::LeftTop });
		UIEx(m_main).AttachComponent(move(listArea), {});
		m_uiModule->BindTextureResources();
		m_main->EnableToolMode(true);

		auto components = UIEx(m_main).FindRenderComponents({ 196, 40 });
		auto ids = GetComponentIDs(components);

		//렌더링 옵션을 바꿔서 BFS 탐색을 하다가 DFS 탐색으로 전환하는 식으로 만든다.
		//DFS 탐색하다가 BFS 탐색하는 것은 일단 보류
		EXPECT_TRUE(ContainsSequence(ids, { 
			RenderTexture, ScrollBar, PatchTextureStd3, PatchTextureStd1, TextureSwitcher }));		
	}

	TEST_F(Integration, RecursivePosition)
	{
		auto [panel1, panel1Ptr] = GetPtrs(CreateComponent<Panel>(UILayout({ 400, 400 }, Origin::Center)));
		auto [panel2, panel2Ptr] = GetPtrs(CreateComponent<Panel>(UILayout{ { 20, 20 }, Origin::Center }));

		UIEx(panel1).AttachComponent(move(panel2), { 40, 40 });
		UIEx(m_main).AttachComponent(move(panel1), { 400, 300 });
		m_uiModule->BindTextureResources();
		m_uiModule->Update(m_timer);

		vector<UIComponent*> outList = UIEx(m_main).FindRenderComponents({ 240, 140 });
		EXPECT_EQ(outList.size(), 3);

		panel2Ptr->ChangeOrigin(Origin::LeftTop);
		m_main->ProcessUpdate(m_timer);

		outList.clear();
		outList = UIEx(m_main).FindRenderComponents({ 239, 140 });
		EXPECT_EQ(outList.size(), 2);

		panel1Ptr->ChangeSize(800, 800); //크기 400에 40위치를 했기 때문에 ratio는 0.1이 된다. 그래서 80
		EXPECT_EQ(panel2Ptr->GetRelativePosition(), XMINT2(80, 80));

		panel1Ptr->SetStateFlag(StateFlag::LockPosOnResize, true);
		panel1Ptr->ChangeSize(400, 400);
		EXPECT_EQ(panel2Ptr->GetRelativePosition(), XMINT2(80, 80));
	}

	//이름을 구역을 만들어서 다른 구역이면 같은 이름을 쓸 수 있게 한다. 그러면 close 같은 이름이 중복이 되어도
	//카피 했을때 다른 구역이라면 close 이름을 쓸 수 있다. 리스트 컴포넌트에서 다른 구역으로 지정하면 이름이 
	//같아도 되니까 코딩할때 이름_1 이런것을 찾지 않아도 된다.
	TEST_F(Integration, Region)
	{
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex2, tex2Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));

		EXPECT_TRUE(UIEx(tex1Ptr).RenameRegion("Region")); //먼저 Region값을 넣어주면 이름이 같아도 되고 나중에 Region을 넣으면 
		EXPECT_TRUE(UIEx(tex2Ptr).RenameRegion("Region")); //Attach 할때 unique 이름으로 만들어 준다.

		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });
		UIEx(m_main).AttachComponent(move(tex2), { 100, 100 });

		EXPECT_EQ(tex2Ptr->GetRegion(), "Region_1"); //이름이 바뀌었다.
		EXPECT_EQ(tex1Ptr->GetName(), "PatchTextureStd1");
		EXPECT_EQ(tex2Ptr->GetName(), "PatchTextureStd1");

		auto [tex3, tex3Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(tex3).Rename("UnChanging Name");
		auto [tex4, tex4Ptr] = GetPtrs(tex3->Clone());

		UIEx(tex1Ptr).AttachComponent(move(tex3), { 100, 100 });
		UIEx(tex2Ptr).AttachComponent(move(tex4), { 100, 100 });

		EXPECT_EQ(tex3Ptr->GetName(), "UnChanging Name");
		EXPECT_EQ(tex4Ptr->GetName(), "UnChanging Name");

		unique_ptr<UIComponent> imgDummy = CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1");
		auto imgDummyPtr = imgDummy.get();
		UIEx(m_main).AttachComponent(move(imgDummy), { 100, 100 });
		EXPECT_TRUE(UIEx(imgDummyPtr).Rename("UnChanging Name"));

		auto [tex5, tex5Ptr] = GetPtrs(tex1Ptr->Clone());
		EXPECT_TRUE(UIEx(tex5).RenameRegion("")); //?!? 데이터가 없는 NameGenerator라서 false가 나온다. generator가 존재하지 않는다면 그냥 바꿔도 무방할것 같다. attach 할때 무결성을 체크하기 때문이다.
		UIEx(m_main).AttachComponent(move(tex5), { 100, 100 });
		EXPECT_TRUE(UIEx(tex5Ptr).FindComponent("UnChanging Name")); //region을 없애고 밑에 노드를 찾으면 찾아진다.

		//region을 1로 해 놓고 그걸 clone해서 다시 그 노드에 붙였을때 clone된 노드 region은 1_1로 붙어야함.
		auto [tex6, tex6Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(m_main).AttachComponent(move(tex6), { 100, 100 });
		EXPECT_TRUE(UIEx(tex6Ptr).RenameRegion("SubRegionName"));
		auto [tex7, tex7Ptr] = GetPtrs(tex6Ptr->Clone());
		UIEx(tex6Ptr).AttachComponent(move(tex7), { 100, 100 });
		EXPECT_EQ(tex7Ptr->GetRegion(), "SubRegionName_1");
	}

	TEST_F(Integration, Rename)
	{
		auto tex9 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9), { 80, 60 });

		UIComponent* component = UIEx(m_main).FindComponent("PatchTextureStd1");
		EXPECT_FALSE(UIEx(component).Rename("PatchTextureStd9"));

		auto newImg9 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		auto failed = UIEx(m_main).AttachComponent(move(newImg9), { 80, 60 });
		EXPECT_TRUE(failed == nullptr);
	}

	TEST_F(Integration, RenameRegion)
	{
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex2, tex2Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));

		UIEx(tex1).AttachComponent(move(tex2), { 100, 100 });
		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });

		//tex2에 RenameRegion을 하면 tex2의 이름은 PatchTextureStd1_1에서 PatchTextureStd1이 된다. 새로운 region으로 바뀌었기 때문에 이름도 처음부터 시작한다.
		EXPECT_EQ(tex2Ptr->GetName(), "PatchTextureStd1_1");
		EXPECT_TRUE(UIEx(tex2Ptr).RenameRegion("newRegion"));
		EXPECT_EQ(tex2Ptr->GetName(), "PatchTextureStd1");

		//RenameRegion에서 region값이 있을때에는 기존의 region 이름만 변경될뿐 안에 name에는 영향을 미치지 않기 때문에
		//이름만 변경하고 끝낸다. 하지만, region값이 "" 일 경우에는 기존의 region에 속해 있으므로, 새로운 Region을 만드는 경우이기
		//때문에 기존의 region을 재구축하고 새로운 region을 만들어야 한다. 그러면 연산하는데 오래 걸리는데 이 RenameRegion은
		//update시 호출하는 함수가 아니기 때문에 게임시에는 속도영향을 주지 않을 것이다. 즉 attach, detach 할때에는 이 연산이
		//안 쓰이고 위에 이름만 교체되는 RenameRegion이 사용되기 때문에 속도문제는 해결된다.

		auto [tex3, tex3Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex4, tex4Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));

		UIEx(m_main).AttachComponent(move(tex3), { 100, 100 });
		EXPECT_EQ(tex3Ptr->GetName(), "PatchTextureStd1_1"); //newRegion이 생기면서 기존의 region에서 _1이 떨어져 나갔기 때문에 _1로 생성됨.
		UIEx(tex2Ptr).AttachComponent(move(tex4), { 100, 100 });
		EXPECT_EQ(tex4Ptr->GetName(), "PatchTextureStd1_1"); //newRegion은 PatchTextureStd1 밖에 없기 때문에 _1 이 생성될 차례임.

		EXPECT_TRUE(UIEx(tex2Ptr).RenameRegion("anotherRegion")); //기존에 region 이름이 있으면 region 이름만 바꾼다.
		EXPECT_FALSE(UIEx(tex2Ptr).RenameRegion(m_main->GetRegion()));	//같은 이름이 있으면 안된다.
	}

	TEST_F(Integration, UINameGenerator)
	{
		unique_ptr<UINameGenerator> generator = make_unique<UINameGenerator>();
		string region = "newRegion";
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion");
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion_1");
		EXPECT_FALSE(generator->IsUnusedRegion("newRegion_1"));

		EXPECT_TRUE(generator->RemoveRegion(region));
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion");
		EXPECT_TRUE(generator->RemoveRegion("newRegion"));
		EXPECT_TRUE(generator->RemoveRegion("newRegion_1"));
		EXPECT_TRUE(generator->IsUnusedRegion(region));

		EXPECT_EQ(generator->MakeNameOf("", "Region", ComponentID::PatchTextureStd1), make_pair("Region", "PatchTextureStd1"));
		EXPECT_FALSE(generator->IsUnusedName("Region", "PatchTextureStd1"));
		EXPECT_TRUE(generator->RemoveName("Region", "PatchTextureStd1"));
		EXPECT_TRUE(generator->IsUnusedName("Region", "PatchTextureStd1"));

		string name = "newName";
		EXPECT_EQ(generator->MakeNameOf(name, "Region", ComponentID::PatchTextureStd1), make_pair("Region", "newName"));
		EXPECT_EQ(generator->MakeNameOf(name, "Region", ComponentID::PatchTextureStd1), make_pair("Region", "newName_1"));
		EXPECT_TRUE(generator->RemoveName("Region", "newName"));
		EXPECT_FALSE(generator->RemoveName("Region", "newName"));
		EXPECT_FALSE(generator->RemoveName("Region", "newName_0"));

		EXPECT_TRUE(generator->RemoveRegion("Region")); //newName_1이 남아 있지만 상관없이 region을 지운다.
		EXPECT_TRUE(generator->IsUnusedRegion("Region"));

		//region을 삭제하면 밑에 딸려있는 name들도 모두 삭제된다. 이때 노드에 있는 이름과 동기화가 안되지만,
		//노드 탐색하면서 이름을 다시 만들어 주면서 동기화가 된다. 여기서 동기화를 시킨다면 generator 클래스가 node 클래스를 알아야 하는데, 그건 이치에 안 맞는 것 같다.

		EXPECT_EQ(generator->MakeNameOf(name, "", ComponentID::PatchTextureStd1), make_pair("", "newName"));
		EXPECT_EQ(generator->MakeNameOf("newName_10", "", ComponentID::PatchTextureStd1), make_pair("", "newName_1"));

		unique_ptr<UINameGenerator> cloneGenerator = generator->Clone();
		EXPECT_TRUE(Compare(generator, cloneGenerator));

		//이름을 rename 할때 _숫자 가 검출되면 이름바꾸기가 안되도록 한다.
		//실제로 attach, detach 할때 이걸 사용해서 하도록 수정하고 name 관련 코드 삭제 한다.
	}

	static UIComponent* TestAttachName(UIComponent* parent, const string& childName)
	{
		auto [tex, texPtr] = GetPtrs(CreateComponent<PatchTextureStd1>("BackImage1"));
		UIEx(parent).AttachComponent(move(tex), { 400, 300 });
		EXPECT_EQ(texPtr->GetName(), childName);
		return texPtr;
	}

	TEST_F(Integration, UniqueName)
	{
		auto texPtr = TestAttachName(m_main, "PatchTextureStd1");
		auto texPtr1 = TestAttachName(m_main, "PatchTextureStd1_1");
		auto texPtr2 = TestAttachName(m_main, "PatchTextureStd1_2");

		//Detach하고 난 이후에는 recycle에 들어가서 다시 재사용 되기 때문에 PatchTextureStd1이 된다.
		UIEx(texPtr).DetachComponent();
		texPtr = TestAttachName(m_main, "PatchTextureStd1");

		//이름을 바꿀때에도 직접적으로 바꾸면 안된다. 자신이 가지고 있는 것을 반납해야 한다.
		EXPECT_FALSE(UIEx(texPtr1).Rename("PatchTextureStd1"));
		EXPECT_TRUE(UIEx(texPtr1).Rename("NoMatchComponentType"));
		auto texPtr3 = TestAttachName(m_main, "PatchTextureStd1_1");

		//recycle에 들어갔는지 확인하기 위해서
		UIEx(texPtr3).DetachComponent();
		m_uiModule->BindTextureResources();

		const wstring filename = L"Test/Data/RWUIModuleTest.json";
		EXPECT_TRUE(m_uiModule->Write(filename));

		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		unique_ptr<UIModule> read = CreateUIModule(filename, m_mockRenderer.get(), srcBinderFilename);

		EXPECT_TRUE(Compare(m_uiModule, read));

		//?!? region값이 다를때 밑에 이름값은 변화가 없어야 하는데 지금은 변하고 있다. 테스트 해야함.
		//?!? 세이브 로드가 끝나고 프로젝트를 UIComponent에서 UIRegistry로 read, write 하는걸로 바꾼후에
		//json 부분에 Write를 Read 정리를 해야겠다.
	}
}