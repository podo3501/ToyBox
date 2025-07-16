#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "../Toy/UserInterface/UINameGenerator.h"
#include "../Toy/UserInterface/UIModule.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"

namespace UserInterfaceTest
{
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

	TEST_F(IntegrationTest, AttachDetachTest)
	{
		auto tex9 = CreateComponent<PatchTextureStd9>(UILayout{ {200, 100}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_main, "PatchTextureStd9_0"), false); //Tex9에는 Attach 안됨 
		EXPECT_EQ(DetachComponentHelper(m_main, "PatchTextureStd9_0"), true); //위에서 PatchTextureStd1를 attach 했다.
		EXPECT_EQ(AttachComponentHelper(m_main, "Main"), true);
		EXPECT_TRUE(UIEx(m_main).FindComponent("PatchTextureStd1_0")); //Detach 하고 이름을 반환했다면 PatchTextureStd1_0 이름이어야 한다.
		EXPECT_EQ(DetachComponentHelper(m_main, "PatchTextureStd1_0"), true);

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

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd1>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage1")));
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd3>(UILayout{ { 100, 36 }, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal")));
		EXPECT_TRUE(VerifyClone(CreateComponent<PatchTextureStd9>(UILayout{ { 220, 190 }, Origin::LeftTop }, "BackImage9")));
		vector<wstring> bindFontKeys{ L"Hangle", L"English" };
		EXPECT_TRUE(VerifyClone(CreateComponent<TextArea>(UILayout{ { 220, 190 }, Origin::LeftTop }, L"<Hangle>테스트 입니다!</Hangle>", bindFontKeys)));
		EXPECT_TRUE(VerifyClone(CreateComponent<TextureSwitcher>(UILayout{ { 220, 190 }, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateComponent<TextureSwitcher>(UILayout{ { 48, 100 }, Origin::Center }, TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateSampleListArea({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		vector<UIComponent*> components = UIEx(panel).GetRenderComponents(position);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponent)
	{
		auto [tex2, tex2Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(tex1).AttachComponent(move(tex2), { 100, 100 });
		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });

		tex1Ptr->Rename("image1"); 
		tex2Ptr->Rename("image2");
		tex1Ptr->RenameRegion("Region1");
		
		EXPECT_FALSE(UIEx(m_main).FindComponent("image1")); //Img1이 다른 Region이라서 찾을 수 없다.
		EXPECT_FALSE(UIEx(m_main).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex1Ptr).FindComponent("Main"));//Img1이 Region이라서 위에 노드는 못 찾는다.
		EXPECT_TRUE(UIEx(tex1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex2Ptr).FindComponent("Main"));
		EXPECT_TRUE(UIEx(tex2Ptr).FindComponent("image1"));

		tex2Ptr->RenameRegion("Region2");

		EXPECT_FALSE(UIEx(tex1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex2Ptr).FindComponent("image1"));

		EXPECT_EQ(UIEx(m_main).GetRegionComponent("Region1"), tex1Ptr);
		EXPECT_FALSE(UIEx(m_main).GetRegionComponent("Region2"));
		EXPECT_EQ(UIEx(tex1Ptr).GetRegionComponent("Region2"), tex2Ptr);
		EXPECT_FALSE(UIEx(tex2Ptr).GetRegionComponent("Region1"));
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		auto tex9_0 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9_0), { 80, 60 });
		m_uiModule->BindTextureResources();
		m_uiModule->Update(m_timer);

		EXPECT_TRUE(CheckComponentCount(m_main, {0, 0}) == 1);
		EXPECT_EQ(CheckComponentCount(m_main, { 100, 100 }), 4);

		auto tex9_1 = CreateComponent<PatchTextureStd9>(UILayout{ {221, 191}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9_1), { 88, 66 });
		m_uiModule->BindTextureResources();
		m_uiModule->Update(m_timer);

		EXPECT_TRUE(CheckComponentCount(m_main, { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		auto tex9 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		UIEx(panel).AttachComponent(move(tex9), { 40, 30 });

		UIEx(m_main).AttachComponent(move(panel), { 400, 300 });
		m_uiModule->BindTextureResources();
		m_uiModule->Update(m_timer);

		UIComponent* component = m_uiModule->FindComponent("PatchTextureStd1_4");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));
		EXPECT_EQ(component->GetArea(), Rectangle(270, 216, 160, 128));
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

	TEST_F(IntegrationTest, MatchesRenderOrder)
	{
		using enum ComponentID;

		auto listArea = CreateSampleListArea({ { 200, 170 }, Origin::LeftTop });
		UIEx(m_main).AttachComponent(move(listArea), {});
		m_uiModule->BindTextureResources();
		m_main->EnableToolMode(true);

		auto components = UIEx(m_main).GetRenderComponents({ 196, 40 });
		auto ids = GetComponentIDs(components);

		//렌더링 옵션을 바꿔서 BFS 탐색을 하다가 DFS 탐색으로 전환하는 식으로 만든다.
		//DFS 탐색하다가 BFS 탐색하는 것은 일단 보류
		EXPECT_TRUE(ContainsSequence(ids, { 
			RenderTexture, ScrollBar, PatchTextureStd3, PatchTextureStd1, TextureSwitcher }));		
	}

	TEST_F(IntegrationTest, RecursivePosition)
	{
		auto [panel1, panel1Ptr] = GetPtrs(CreateComponent<Panel>(UILayout({ 400, 400 }, Origin::Center)));
		auto [panel2, panel2Ptr] = GetPtrs(CreateComponent<Panel>(UILayout{ { 20, 20 }, Origin::Center }));

		UIEx(panel1).AttachComponent(move(panel2), { 40, 40 });
		UIEx(m_main).AttachComponent(move(panel1), { 400, 300 });
		m_uiModule->BindTextureResources();
		m_uiModule->Update(m_timer);

		vector<UIComponent*> outList = UIEx(m_main).GetRenderComponents({ 240, 140 });
		EXPECT_EQ(outList.size(), 3);

		panel2Ptr->ChangeOrigin(Origin::LeftTop);
		m_main->ProcessUpdate(m_timer);

		outList.clear();
		outList = UIEx(m_main).GetRenderComponents({ 239, 140 });
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
	TEST_F(IntegrationTest, Region)
	{
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex2, tex2Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));

		tex1Ptr->RenameRegion("Region_0"); //먼저 Region값을 넣어주면 이름이 같아도 되고 나중에 Region을 넣으면 
		tex2Ptr->RenameRegion("Region_0"); //Attach 할때 unique 이름으로 만들어 준다.

		UIEx(m_main).AttachComponent(move(tex1), { 100, 100 });
		UIEx(m_main).AttachComponent(move(tex2), { 100, 100 });

		EXPECT_EQ(tex2Ptr->GetRegion(), "Region_1"); //이름이 바뀌었다.
		EXPECT_EQ(tex1Ptr->GetName(), "PatchTextureStd1_0");
		EXPECT_EQ(tex2Ptr->GetName(), "PatchTextureStd1_0");

		auto [tex3, tex3Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		tex3->Rename("UnChanging Name");
		auto [tex4, tex4Ptr] = GetPtrs(tex3->Clone());

		UIEx(tex1Ptr).AttachComponent(move(tex3), { 100, 100 });
		UIEx(tex2Ptr).AttachComponent(move(tex4), { 100, 100 });

		EXPECT_EQ(tex3Ptr->GetName(), "UnChanging Name");
		EXPECT_EQ(tex4Ptr->GetName(), "UnChanging Name");

		unique_ptr<UIComponent> imgDummy = CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1");
		auto imgDummyPtr = imgDummy.get();
		UIEx(m_main).AttachComponent(move(imgDummy), { 100, 100 });
		EXPECT_TRUE(imgDummyPtr->Rename("UnChanging Name"));

		auto [tex5, tex5Ptr] = GetPtrs(tex1Ptr->Clone());
		UIEx(m_main).AttachComponent(move(tex5), { 100, 100 });
		EXPECT_TRUE(UIEx(tex5Ptr).FindComponent("UnChanging Name"));
	}

	TEST_F(IntegrationTest, Rename)
	{
		auto tex9 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_main).AttachComponent(move(tex9), { 80, 60 });

		UIComponent* component = UIEx(m_main).FindComponent("PatchTextureStd1_0");
		EXPECT_FALSE(component->Rename("PatchTextureStd9_0")); //같은 이름이 있으면 rename이 되지 않는다.

		auto newImg9 = CreateComponent<PatchTextureStd9>(UILayout{ {220, 190}, Origin::LeftTop }, "BackImage9");
		auto failed = UIEx(m_main).AttachComponent(move(newImg9), { 80, 60 });
		EXPECT_TRUE(failed == nullptr);
	}

	TEST_F(IntegrationTest, UINameGenerator)
	{
		unique_ptr<UINameGenerator> generator = make_unique<UINameGenerator>();
		string region = "newRegion";
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion");
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion_1");
		
		EXPECT_TRUE(generator->RemoveRegion(region));
		EXPECT_EQ(generator->MakeRegionOf(region), "newRegion");
		EXPECT_TRUE(generator->RemoveRegion("newRegion"));
		EXPECT_TRUE(generator->RemoveRegion("newRegion_1"));
		EXPECT_TRUE(generator->IsUniqueRegion(region));

		EXPECT_EQ(generator->MakeNameOf("", "Region", ComponentID::PatchTextureStd1), "PatchTextureStd1");
		EXPECT_FALSE(generator->IsUniqueName("Region", "PatchTextureStd1"));
		EXPECT_TRUE(generator->RemoveName("Region", "PatchTextureStd1"));
		EXPECT_TRUE(generator->IsUniqueName("Region", "PatchTextureStd1"));

		string name = "newName";
		EXPECT_EQ(generator->MakeNameOf(name, "Region", ComponentID::PatchTextureStd1), "newName");
		EXPECT_EQ(generator->MakeNameOf(name, "Region", ComponentID::PatchTextureStd1), "newName_1");
		EXPECT_TRUE(generator->RemoveName("Region", "newName"));
		EXPECT_FALSE(generator->RemoveName("Region", "newName"));
		EXPECT_FALSE(generator->RemoveName("Region", "newName_0"));
		EXPECT_TRUE(generator->RemoveName("Region", "newName_1"));

		EXPECT_TRUE(generator->RemoveRegion("Region"));
		EXPECT_TRUE(generator->IsUniqueRegion("Region"));

		//region을 삭제하면 밑에 딸려있는 name들도 모두 삭제된다. 하지만 node에서 노드 탐색하면서 이름을 
		//다시 넣어주기 때문에 삭제된 이름들은 다른 region에 붙게 된다.

		//generator->MakeRegionOf(region);


		//"" 이름없는 region일 경우 테스트 추가
		//이름_1이 있는데 이름_1을 추가하면 이름_2가 되는지 확인
		//rename의 경우 region이 바뀌면 밑에 이름들은 실패가 반환되면 안되기 때문에 강제 rename이 필요할껀데
		//사용자가 rename할 경우에는 이름이 있는 경우는 실패를 반환하는게 직관적일듯.


		//rename 하는 로직추가
		//generator안에 모든 이름이 있지만 여기서 이름을 수정한다고 해도 실제로 component에 이름이 수정이
		//안되기 때문에 rename이라는 것은 노드에서 있어야 하고 거기서 remove와 insert를 통해서 이름을 rename
		//하는 식으로 바꿔야 한다.

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

	TEST_F(IntegrationTest, UniqueName)
	{
		auto texPtr0 = TestAttachName(m_main, "PatchTextureStd1_0");
		auto texPtr1 = TestAttachName(m_main, "PatchTextureStd1_1");
		auto texPtr2 = TestAttachName(m_main, "PatchTextureStd1_2");

		//Detach하고 난 이후에는 _0이 recycle에 들어가서 다시 재사용 되기 때문에 _0이 된다.
		UIEx(texPtr0).DetachComponent();
		texPtr0 = TestAttachName(m_main, "PatchTextureStd1_0");

		//이름을 바꿀때에도 직접적으로 바꾸면 안된다. 자신이 가지고 있는 것을 반납해야 한다.
		UIEx(texPtr1).Rename("NoMatchComponentType_0");
		auto texPtr3 = TestAttachName(m_main, "PatchTextureStd1_1");

		//recycle에 들어갔는지 확인하기 위해서
		UIEx(texPtr3).DetachComponent();
		m_uiModule->BindTextureResources();

		const wstring filename = L"Test/Data/RWUIModuleTest.json";
		EXPECT_TRUE(m_uiModule->Write(filename));

		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		unique_ptr<UIModule> read = CreateUIModule(filename, m_renderer.get(), srcBinderFilename);

		EXPECT_TRUE(CompareUniquePtr(m_uiModule, read));

		//?!? region값이 다를때 밑에 이름값은 변화가 없어야 하는데 지금은 변하고 있다. 테스트 해야함.
		//?!? 세이브 로드가 끝나고 프로젝트를 UIComponent에서 UIRegistry로 read, write 하는걸로 바꾼후에
		//json 부분에 Write를 Read 정리를 해야겠다.
	}
}