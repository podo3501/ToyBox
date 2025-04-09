#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"

namespace UserInterfaceTest
{
	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		auto patchTex1 = CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1");
		UIComponent* component = UIEx(panel).FindComponent(componentName);
		return UIEx(component).AttachComponent(move(patchTex1), { 10, 10 }) ? false : true;
	}

	static bool DetachComponentHelper(UIComponent* panel, const string& componentName) noexcept 
	{
		UIComponent* component = UIEx(panel).FindComponent(componentName);
		auto [detach, parent] = UIEx(component).DetachComponent();
		return detach != nullptr;
	}

	TEST_F(IntegrationTest, AttachDetachTest)
	{
		auto tex9 = CreatePatchTexture9({ {200, 100}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(tex9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "PatchTexture9_0"), false);	//9방향 이미지에는 attach 불가
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "PatchTexture1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "PatchTexture1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "PatchTexture1_9"), true); //위에서 PatchTexture1를 attach 했다.

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "PatchTexture9_0"), true); //위에서 PatchTexture1를 attach 했다.

		auto [tex1, tex1Ptr] = GetPtrs(CreatePatchTexture1({ {200, 100}, Origin::LeftTop }, "BackImage1"));
		auto tex2 = CreatePatchTexture1({ {110, 60}, Origin::LeftTop }, "BackImage1");
		UIEx(tex1).AttachComponent(move(tex2), { 100, 50 });	//중점에 attach 한다.
		UIEx(m_panel).AttachComponent(move(tex1), { 100, 100 });
		m_panel->ProcessUpdate(m_timer);

		EXPECT_EQ(UIEx(tex1Ptr).GetChildrenBoundsSize(), XMUINT2(210, 110));
		auto [detached, parent] = UIEx(tex1Ptr).DetachComponent();
		detached->ProcessUpdate(m_timer);
		EXPECT_EQ(UIEx(detached).GetChildrenBoundsSize(), XMUINT2(210, 110));
	}

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreatePatchTexture1({ {220, 190}, Origin::LeftTop }, "BackImage1")));
		EXPECT_TRUE(VerifyClone(CreatePatchTexture3({ { 100, 36 }, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal")));
		EXPECT_TRUE(VerifyClone(CreatePatchTexture9({ { 220, 190 }, Origin::LeftTop }, "BackImage9")));
		vector<wstring> bindFontKeys{ L"Hangle", L"English" };
		EXPECT_TRUE(VerifyClone(CreateTextArea({ { 220, 190 }, Origin::LeftTop }, L"<Hangle>테스트 입니다!</Hangle>", bindFontKeys)));
		EXPECT_TRUE(VerifyClone(CreateTextureSwitcher({ { 220, 190 }, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateTextureSwitcher({ { 48, 100 }, Origin::Center }, TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateSampleListArea({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		vector<UIComponent*> components = UIEx(panel).GetComponents(position);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponent)
	{
		auto [tex2, tex2Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex1, tex1Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(tex1).AttachComponent(move(tex2), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(tex1), { 100, 100 });

		tex1Ptr->Rename("image1"); 
		tex2Ptr->Rename("image2");
		tex1Ptr->RenameRegion("Region1");
		
		EXPECT_FALSE(UIEx(m_panel).FindComponent("image1")); //Img1이 다른 Region이라서 찾을 수 없다.
		EXPECT_FALSE(UIEx(m_panel).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex1Ptr).FindComponent("Main"));//Img1이 Region이라서 위에 노드는 못 찾는다.
		EXPECT_TRUE(UIEx(tex1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex2Ptr).FindComponent("Main"));
		EXPECT_TRUE(UIEx(tex2Ptr).FindComponent("image1"));

		tex2Ptr->RenameRegion("Region2");

		EXPECT_FALSE(UIEx(tex1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(tex2Ptr).FindComponent("image1"));

		EXPECT_EQ(UIEx(m_panel).GetRegionComponent("Region1"), tex1Ptr);
		EXPECT_FALSE(UIEx(m_panel).GetRegionComponent("Region2"));
		EXPECT_EQ(UIEx(tex1Ptr).GetRegionComponent("Region2"), tex2Ptr);
		EXPECT_FALSE(UIEx(tex2Ptr).GetRegionComponent("Region1"));
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		auto tex9_0 = CreatePatchTexture9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(tex9_0), { 80, 60 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_EQ(CheckComponentCount(m_panel.get(), { 100, 100 }), 4);

		auto tex9_1 = CreatePatchTexture9({ {221, 191}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(tex9_1), { 88, 66 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		auto tex9 = CreatePatchTexture9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		UIEx(panel).AttachComponent(move(tex9), { 40, 30 });
		UIEx(m_panel).AttachComponent(move(panel), { 400, 300 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);

		UIComponent* component = UIEx(m_panel).FindComponent("PatchTexture1_4");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));
		EXPECT_EQ(component->GetArea(), Rectangle(270, 216, 160, 128));
	}

	TEST_F(IntegrationTest, RecursivePosition)
	{
		std::unique_ptr<Panel> panel1 = make_unique<Panel>("Panel1", UILayout({ 400, 400 }, Origin::Center));
		auto [panel2, panel2Ptr] = GetPtrs(make_unique<Panel>("Panel2", UILayout({ 20, 20 }, Origin::Center)));

		UIEx(panel1).AttachComponent(move(panel2), { 40, 40 });
		UIEx(m_panel).AttachComponent(move(panel1), { 400, 300 });
		m_panel->ProcessUpdate(m_timer);

		vector<UIComponent*> outList = UIEx(m_panel).GetComponents({ 240, 140 });
		EXPECT_EQ(outList.size(), 3);

		panel2Ptr->ChangeOrigin(Origin::LeftTop);
		m_panel->ProcessUpdate(m_timer);

		outList.clear();
		outList = UIEx(m_panel).GetComponents({ 239, 140 });
		EXPECT_EQ(outList.size(), 2);

		//사이즈가 바뀌었을때 값이 어떻게 바뀌는지 테스트
	}

	//이름을 구역을 만들어서 다른 구역이면 같은 이름을 쓸 수 있게 한다. 그러면 close 같은 이름이 중복이 되어도
	//카피 했을때 다른 구역이라면 close 이름을 쓸 수 있다. 리스트 컴포넌트에서 다른 구역으로 지정하면 이름이 
	//같아도 되니까 코딩할때 이름_1 이런것을 찾지 않아도 된다.
	TEST_F(IntegrationTest, Region)
	{
		auto [tex1, tex1Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [tex2, tex2Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1"));

		tex1Ptr->RenameRegion("Region_0"); //먼저 Region값을 넣어주면 이름이 같아도 되고 나중에 Region을 넣으면 
		tex2Ptr->RenameRegion("Region_0"); //Attach 할때 unique 이름으로 만들어 준다.

		UIEx(m_panel).AttachComponent(move(tex1), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(tex2), { 100, 100 });

		EXPECT_EQ(tex2Ptr->GetRegion(), "Region_1"); //이름이 바뀌었다.
		EXPECT_EQ(tex1Ptr->GetName(), "PatchTexture1_0");
		EXPECT_EQ(tex2Ptr->GetName(), "PatchTexture1_0");

		auto [tex3, tex3Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		tex3->Rename("UnChanging Name");
		auto [tex4, tex4Ptr] = GetPtrs(tex3->Clone());

		UIEx(tex1Ptr).AttachComponent(move(tex3), { 100, 100 });
		UIEx(tex2Ptr).AttachComponent(move(tex4), { 100, 100 });

		EXPECT_EQ(tex3Ptr->GetName(), "UnChanging Name");
		EXPECT_EQ(tex4Ptr->GetName(), "UnChanging Name");

		unique_ptr<UIComponent> imgDummy = CreatePatchTexture1({ {64, 64}, Origin::LeftTop }, "BackImage1");
		auto imgDummyPtr = imgDummy.get();
		UIEx(m_panel).AttachComponent(move(imgDummy), { 100, 100 });
		EXPECT_TRUE(imgDummyPtr->Rename("UnChanging Name"));

		auto [tex5, tex5Ptr] = GetPtrs(tex1Ptr->Clone());
		UIEx(m_panel).AttachComponent(move(tex5), { 100, 100 });
		EXPECT_TRUE(UIEx(tex5Ptr).FindComponent("UnChanging Name"));
	}

	TEST_F(IntegrationTest, Rename)
	{
		auto tex9 = CreatePatchTexture9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(tex9), { 80, 60 });

		UIComponent* component = UIEx(m_panel).FindComponent("PatchTexture1_0");
		EXPECT_FALSE(component->Rename("PatchTexture9_0")); //같은 이름이 있으면 rename이 되지 않는다.

		auto newImg9 = CreatePatchTexture9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		auto failed = UIEx(m_panel).AttachComponent(move(newImg9), { 80, 60 }); //같은 컴포넌트를 attach하면 내부적으로 이름을 생성해 준다.
		EXPECT_TRUE(failed == nullptr);
	}
}