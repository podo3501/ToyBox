#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureLoadBinder.h"
#include "../TestHelper.h"

namespace UserInterfaceTest
{
	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		auto imgGrid1 = CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1");
		UIComponent* component = UIEx(panel).FindComponent(componentName);
		return UIEx(component).AttachComponent(move(imgGrid1), { 10, 10 }) ? false : true;
	}

	static bool DetachComponentHelper(UIComponent* panel, const string& componentName) noexcept 
	{
		UIComponent* component = UIEx(panel).FindComponent(componentName);
		auto [detach, parent] = UIEx(component).DetachComponent();
		return detach != nullptr;
	}

	TEST_F(IntegrationTest, AttachDetachTest)
	{
		auto img9 = CreateImageGrid9({ {200, 100}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid9_0"), false);	//9방향 이미지에는 attach 불가
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_9"), true); //위에서 ImgGrid1를 attach 했다.

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid9_0"), true); //위에서 ImgGrid1를 attach 했다.

		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {200, 100}, Origin::LeftTop }, "BackImage1"));
		auto img2 = CreateImageGrid1({ {110, 60}, Origin::LeftTop }, "BackImage1");
		UIEx(img1).AttachComponent(move(img2), { 100, 50 });	//중점에 attach 한다.
		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		m_panel->ProcessUpdate(m_timer);

		EXPECT_EQ(UIEx(img1Ptr).GetTotalChildSize(), XMUINT2(210, 110));
		auto [detached, parent] = UIEx(img1Ptr).DetachComponent();
		detached->ProcessUpdate(m_timer);
		EXPECT_EQ(UIEx(detached).GetTotalChildSize(), XMUINT2(210, 110));
	}

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreateImageGrid1({ {220, 190}, Origin::LeftTop }, "BackImage1")));
		EXPECT_TRUE(VerifyClone(CreateImageGrid3(DirectionType::Horizontal, { { 100, 36 }, Origin::LeftTop }, "ScrollButton3_H_Normal")));
		EXPECT_TRUE(VerifyClone(CreateImageGrid9({ { 220, 190 }, Origin::LeftTop }, "BackImage9")));
		vector<wstring> bindFontKeys{ L"Hangle", L"English" };
		EXPECT_TRUE(VerifyClone(CreateTextArea({ { 220, 190 }, Origin::LeftTop }, L"<Hangle>테스트 입니다!</Hangle>", bindFontKeys)));
		EXPECT_TRUE(VerifyClone(CreateImageSwitcher({ { 220, 190 }, Origin::Center }, ImagePart::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateImageSwitcher({ { 48, 100 }, Origin::Center }, ImagePart::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal)));
		EXPECT_TRUE(VerifyClone(CreateSampleListArea({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		vector<UIComponent*> components = UIEx(panel).GetComponents(position);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponent)
	{
		auto [img2, img2Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		UIEx(img1).AttachComponent(move(img2), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });

		img1Ptr->Rename("image1"); 
		img2Ptr->Rename("image2");
		img1Ptr->RenameRegion("Region1");
		
		EXPECT_FALSE(UIEx(m_panel).FindComponent("image1")); //Img1이 다른 Region이라서 찾을 수 없다.
		EXPECT_FALSE(UIEx(m_panel).FindComponent("image2"));
		EXPECT_FALSE(UIEx(img1Ptr).FindComponent("Main"));//Img1이 Region이라서 위에 노드는 못 찾는다.
		EXPECT_TRUE(UIEx(img1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(img2Ptr).FindComponent("Main"));
		EXPECT_TRUE(UIEx(img2Ptr).FindComponent("image1"));

		img2Ptr->RenameRegion("Region2");

		EXPECT_FALSE(UIEx(img1Ptr).FindComponent("image2"));
		EXPECT_FALSE(UIEx(img2Ptr).FindComponent("image1"));

		EXPECT_EQ(UIEx(m_panel).GetRegionComponent("Region1"), img1Ptr);
		EXPECT_FALSE(UIEx(m_panel).GetRegionComponent("Region2"));
		EXPECT_EQ(UIEx(img1Ptr).GetRegionComponent("Region2"), img2Ptr);
		EXPECT_FALSE(UIEx(img2Ptr).GetRegionComponent("Region1"));
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		auto img9_0 = CreateImageGrid9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(img9_0), { 80, 60 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_EQ(CheckComponentCount(m_panel.get(), { 100, 100 }), 4);

		auto img9_1 = CreateImageGrid9({ {221, 191}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(img9_1), { 88, 66 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		auto img9 = CreateImageGrid9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		UIEx(panel).AttachComponent(move(img9), { 40, 30 });
		UIEx(m_panel).AttachComponent(move(panel), { 400, 300 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));
		m_panel->ProcessUpdate(m_timer);

		UIComponent* component = UIEx(m_panel).FindComponent("ImageGrid1_4");
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
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		auto [img2, img2Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1"));

		img1Ptr->RenameRegion("Region_0"); //먼저 Region값을 넣어주면 이름이 같아도 되고 나중에 Region을 넣으면 
		img2Ptr->RenameRegion("Region_0"); //Attach 할때 unique 이름으로 만들어 준다.

		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(img2), { 100, 100 });

		EXPECT_EQ(img2Ptr->GetRegion(), "Region_1"); //이름이 바뀌었다.
		EXPECT_EQ(img1Ptr->GetName(), "ImageGrid1_0");
		EXPECT_EQ(img2Ptr->GetName(), "ImageGrid1_0");

		auto [img3, img3Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1"));
		img3->Rename("UnChanging Name");
		auto [img4, img4Ptr] = GetPtrs(img3->Clone());

		UIEx(img1Ptr).AttachComponent(move(img3), { 100, 100 });
		UIEx(img2Ptr).AttachComponent(move(img4), { 100, 100 });

		EXPECT_EQ(img3Ptr->GetName(), "UnChanging Name");
		EXPECT_EQ(img4Ptr->GetName(), "UnChanging Name");

		unique_ptr<UIComponent> imgDummy = CreateImageGrid1({ {64, 64}, Origin::LeftTop }, "BackImage1");
		auto imgDummyPtr = imgDummy.get();
		UIEx(m_panel).AttachComponent(move(imgDummy), { 100, 100 });
		EXPECT_TRUE(imgDummyPtr->Rename("UnChanging Name"));

		auto [img5, img5Ptr] = GetPtrs(img1Ptr->Clone());
		UIEx(m_panel).AttachComponent(move(img5), { 100, 100 });
		EXPECT_TRUE(UIEx(img5Ptr).FindComponent("UnChanging Name"));
	}

	TEST_F(IntegrationTest, Rename)
	{
		auto img9 = CreateImageGrid9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		UIComponent* component = UIEx(m_panel).FindComponent("ImageGrid1_0");
		EXPECT_FALSE(component->Rename("ImageGrid9_0")); //같은 이름이 있으면 rename이 되지 않는다.

		auto newImg9 = CreateImageGrid9({ {220, 190}, Origin::LeftTop }, "BackImage9");
		auto failed = UIEx(m_panel).AttachComponent(move(newImg9), { 80, 60 }); //같은 컴포넌트를 attach하면 내부적으로 이름을 생성해 준다.
		EXPECT_TRUE(failed == nullptr);
	}

	//////////////////////////////////////////////////////////

	static void TestLoadTextureFromFile(IRenderer* renderer, TextureLoadBinder* loadBinder, const wstring& filename)
	{
		loadBinder->AddTexture(filename);
		renderer->LoadTextureBinder(loadBinder);
		auto sourceInfo = loadBinder->GetSourceInfo(filename);
		EXPECT_TRUE(sourceInfo);
	}

	TEST_F(IntegrationTest, TextureLoadBinder)
	{
		auto loadBinder = make_unique<TextureLoadBinder>();

		const wstring& sampleFilename = L"UI/SampleTexture/Sample_0.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), sampleFilename);

		const wstring& optionFilename = L"UI/SampleTexture/Option.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), optionFilename);
	}

	TEST_F(IntegrationTest, TextureSourceBinder)
	{
		const wstring& filename = L"UI/SampleTexture/Sample_0.png";
		EXPECT_TRUE((GetTextureArea(m_sourceBinder.get(), "BackImage1", 0) == Rectangle{10, 10, 64, 64})); //?!? 테스트 밖에 함수가 사용되지 않고 있다.
		TextureSourceInfo sourceInfo{ filename, ImagePart::One, {{10, 10, 64, 64}} };
		EXPECT_EQ(m_sourceBinder->GetBindingKey(sourceInfo), "BackImage1");

		vector<TextureSourceInfo> horzAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeH);
		EXPECT_TRUE(!horzAreas.empty() && horzAreas[0].sources.size() == 3);
		vector<TextureSourceInfo> vertAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeV);
		EXPECT_NE(horzAreas.size(), vertAreas.size());

		TextureSourceInfo testSourceInfo{ L"TestTexFilename.json", ImagePart::One, {} };
		TextureFontInfo testFontInfo{ L"TestFontFilename.json" };
		EXPECT_TRUE(m_sourceBinder->InsertFontKey(L"Test", testFontInfo));
		EXPECT_TRUE(m_sourceBinder->InsertTextureKey("Test", testSourceInfo));
		EXPECT_TRUE(m_sourceBinder->ModifyFontKey(L"Test", L"NewTest"));
		EXPECT_TRUE(m_sourceBinder->ModifyTextureKey("Test", "NewTest"));
		EXPECT_TRUE(m_sourceBinder->RemoveKeyByFilename(L"TestFontFilename.json"));
		EXPECT_TRUE(m_sourceBinder->RemoveKeyByFilename(L"TestTexFilename.json"));

		EXPECT_TRUE(m_sourceBinder->GetTextureKeys(ImagePart::ThreeH).size());

		TestSourceBinderWriteRead(m_sourceBinder);

		auto sourceBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
		m_renderer->LoadTextureBinder(sourceBinder.get());
		
		EXPECT_EQ(*m_sourceBinder, *sourceBinder);
	}

	//////////////////////////////////////////////////////////
	TEST_F(IntegrationTest, UndoRedo)
	{
		CommandList cmdList;
		vector<unique_ptr<UIComponent>> history;
		UIComponent* panel = m_panel.get();
		CaptureSnapshot(cmdList, history);

		auto [img9, img9Ptr] = GetPtrs(CreateImageGrid9({ {170, 120}, Origin::Center }, "BackImage9"));
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::Center }, "BackImage1"));

		cmdList.AttachComponent(m_panel.get(), move(img1), { 111, 222 });
		CaptureSnapshot(cmdList, history);

		cmdList.SetRelativePosition(img1Ptr, { 123, 234 });
		CaptureSnapshot(cmdList, history);

		cmdList.SetSize(img1Ptr, { 32, 32 });
		CaptureSnapshot(cmdList, history);

		cmdList.RenameRegion(img1Ptr, "region");
		CaptureSnapshot(cmdList, history);

		cmdList.Rename(img1Ptr, "img1");
		CaptureSnapshot(cmdList, history);

		cmdList.DetachComponent(img1Ptr);
		CaptureSnapshot(cmdList, history);

		VerifyUndoRedo(cmdList, history);
	}
}