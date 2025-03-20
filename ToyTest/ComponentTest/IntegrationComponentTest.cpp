#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"

namespace UserInterfaceTest
{
	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		auto imgGrid1 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
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
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 200, 100 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid9_0"), false);	//9방향 이미지에는 attach 불가
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_9"), true); //위에서 ImgGrid1를 attach 했다.

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid9_0"), true); //위에서 ImgGrid1를 attach 했다.

		auto [img1, img1Ptr] = GetPtrs(CreateSampleImageGrid1({ { 200, 100 }, Origin::LeftTop }));
		auto img2 = CreateSampleImageGrid1({ { 110, 60 }, Origin::LeftTop });
		UIEx(img1).AttachComponent(move(img2), { 100, 50 });	//중점에 attach 한다.
		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		m_panel->ProcessUpdate(m_timer);

		EXPECT_EQ(UIEx(img1Ptr).GetTotalChildSize(), XMUINT2(210, 110));
		auto [detached, parent] = UIEx(img1Ptr).DetachComponent();
		detached->ProcessUpdate(m_timer);
		EXPECT_EQ(UIEx(detached).GetTotalChildSize(), XMUINT2(210, 110));
	}

	template <typename T>
	static bool VerifyClone(IRenderer* renderer, unique_ptr<T> original) 
	{
		if (!original) return false;
		unique_ptr<UIComponent> origin = move(original); //업 캐스트 한다. unique_ptr은 업 캐스트가 안되고 move를 통해서만 된다.
		EXPECT_TRUE(renderer->LoadComponent(origin.get()));
		auto clone = origin->Clone();
		
		return CompareUniquePtr(origin, clone);
	}

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleImageGrid1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleImageGrid3(DirectionType::Horizontal, { { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleTextArea({ { 220, 190 }, Origin::LeftTop }, L"<Hangle>테스트 입니다!</Hangle>")));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleButton1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleButton3(DirectionType::Horizontal, { { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(m_renderer.get(), CreateSampleListArea1({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		vector<UIComponent*> components = UIEx(panel).GetComponents(position);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponent)
	{
		auto [img2, img2Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop }));
		auto [img1, img1Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop }));
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
		unique_ptr<UIComponent> img9_0 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9_0), { 80, 60 });
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 100, 100 }) == 4);

		unique_ptr<UIComponent> img9_1 = CreateSampleImageGrid9({ { 221, 191 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9_1), { 88, 66 });
		m_panel->ProcessUpdate(m_timer);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		UIEx(panel).AttachComponent(move(img9), { 40, 30 });
		UIEx(m_panel).AttachComponent(move(panel), { 400, 300 });
		m_panel->ProcessUpdate(m_timer);

		UIComponent* component = UIEx(m_panel).FindComponent("ImageGrid1_4");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));
		EXPECT_EQ(component->GetArea(), Rectangle(270, 216, 160, 128));
	}

	//이름을 구역을 만들어서 다른 구역이면 같은 이름을 쓸 수 있게 한다. 그러면 close 같은 이름이 중복이 되어도
	//카피 했을때 다른 구역이라면 close 이름을 쓸 수 있다. 리스트 컴포넌트에서 다른 구역으로 지정하면 이름이 
	//같아도 되니까 코딩할때 이름_1 이런것을 찾지 않아도 된다.
	TEST_F(IntegrationTest, Region)
	{
		auto [img1, img1Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop }));
		auto [img2, img2Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop }));

		img1Ptr->RenameRegion("Region_0"); //먼저 Region값을 넣어주면 이름이 같아도 되고 나중에 Region을 넣으면 
		img2Ptr->RenameRegion("Region_0"); //Attach 할때 unique 이름으로 만들어 준다.

		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(img2), { 100, 100 });

		EXPECT_EQ(img2Ptr->GetRegion(), "Region_1"); //이름이 바뀌었다.
		EXPECT_EQ(img1Ptr->GetName(), "ImageGrid1_0");
		EXPECT_EQ(img2Ptr->GetName(), "ImageGrid1_0");

		auto [img3, img3Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop }));
		img3->Rename("UnChanging Name");
		auto [img4, img4Ptr] = GetPtrs(img3->Clone());

		UIEx(img1Ptr).AttachComponent(move(img3), { 100, 100 });
		UIEx(img2Ptr).AttachComponent(move(img4), { 100, 100 });

		EXPECT_EQ(img3Ptr->GetName(), "UnChanging Name");
		EXPECT_EQ(img4Ptr->GetName(), "UnChanging Name");

		unique_ptr<UIComponent> imgDummy = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto imgDummyPtr = imgDummy.get();
		UIEx(m_panel).AttachComponent(move(imgDummy), { 100, 100 });
		EXPECT_TRUE(imgDummyPtr->Rename("UnChanging Name"));

		auto [img5, img5Ptr] = GetPtrs(img1Ptr->Clone());
		UIEx(m_panel).AttachComponent(move(img5), { 100, 100 });
		EXPECT_TRUE(UIEx(img5Ptr).FindComponent("UnChanging Name"));
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

	TEST_F(IntegrationTest, Rename)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		UIComponent* component = UIEx(m_panel).FindComponent("ImageGrid1_0");
		EXPECT_FALSE(component->Rename("ImageGrid9_0")); //같은 이름이 있으면 rename이 되지 않는다.

		unique_ptr<UIComponent> newImg9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		auto failed = UIEx(m_panel).AttachComponent(move(newImg9), { 80, 60 }); //같은 컴포넌트를 attach하면 내부적으로 이름을 생성해 준다.
		EXPECT_TRUE(failed == nullptr);
	}

	//////////////////////////////////////////////////////////

	TEST_F(IntegrationTest, TextureSourceBinder)
	{
		EXPECT_TRUE(m_renderer->LoadTextureBinder(m_sourceBinder.get()));

		const wstring& filename = L"UI/SampleTexture/Sample_0.png";
		EXPECT_TRUE((m_sourceBinder->GetArea("BackImage1", 0) == Rectangle{ 10, 10, 64, 64 })); //?!? 테스트 밖에 함수가 사용되지 않고 있다.
		TextureSourceInfo sourceInfo{ filename, ImagePart::One, {{10, 10, 64, 64}} };
		EXPECT_EQ(m_sourceBinder->GetBindingKey(sourceInfo), "BackImage1");

		vector<TextureSourceInfo> horzAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeH);
		EXPECT_TRUE(!horzAreas.empty() && horzAreas[0].sources.size() == 3);
		vector<TextureSourceInfo> vertAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeV);
		EXPECT_NE(horzAreas.size(), vertAreas.size());
	}

	//////////////////////////////////////////////////////////
	TEST_F(IntegrationTest, UndoRedo)
	{
		CommandList cmdList;
		vector<unique_ptr<UIComponent>> history;
		UIComponent* panel = m_panel.get();
		CaptureSnapshot(cmdList, history);

		auto [img9, img9Ptr] = GetPtrs(CreateSampleImageGrid9({ {170, 120}, Origin::Center }));
		auto [img1, img1Ptr] = GetPtrs(CreateSampleImageGrid1({ { 64, 64 }, Origin::Center }));

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

		cmdList.SetSource(img1Ptr, { 11, 11, 32, 32 }); //원래는{ 10, 10, 64, 64 }
		CaptureSnapshot(cmdList, history);

		cmdList.DetachComponent(img1Ptr);
		CaptureSnapshot(cmdList, history);

		cmdList.AttachComponent(m_panel.get(), move(img9), { 200, 210 });
		SourceDivider srcDivider{ img9Ptr->GetSourceAnd4Divider() };
		srcDivider.list = { 20, 44, 26, 48 };
		img9Ptr->SetSourceAnd4Divider(srcDivider);
		CaptureSnapshot(cmdList, history);

		VerifyUndoRedo(cmdList, history);
	}
}