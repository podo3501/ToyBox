#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"
#include "../Toy/UserInterface/Component/ScrollSlider.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/Container.h"
#include "../Toy/InputManager.h"
#include "../Toy/Utility.h"

namespace UserInterfaceTest
{
	static void TestContainer_Scroll(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 92, 50, 108, 57 }, { 92, 57, 108, 143 }, { 92, 143, 108, 150 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_Pressed_V"));
	}

	TEST_F(ComplexComponentTest, Container_Scroll)
	{
		auto [scrollContainer, scrollContainerPtr] = GetPtrs(CreateScrollContainer({ {16, 100}, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(scrollContainer), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		testing::MockFunction<void(KeyState)> mockOnPress;
		scrollContainerPtr->AddPressCB(mockOnPress.AsStdFunction());

		EXPECT_CALL(mockOnPress, Call(KeyState::Pressed)).Times(1); //Pressed 인자를 넣어서 한번 호출할 것을 기대
		EXPECT_CALL(mockOnPress, Call(KeyState::Held)).Times(1);

		MockMouseInput(100, 100, true); //Pressed
		CallMockRender(TestContainer_Scroll, 3);

		MockMouseInput(110, 110, true); //영역에는 벗어났지만 holdToKeepPressed 옵션이 있기 때문에 Held가 되어야한다.
		CallMockRender(TestContainer_Scroll, 3);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	TEST_F(ComplexComponentTest, ListArea)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea1({ { 150, 120 }, Origin::Center }));
		auto scrollBarPtr = UIEx(listAreaPtr).FindComponent<ScrollBar*>("ScrollBar_0");
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), listAreaPtr, 5));
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
		auto preSizeX = listAreaPtr->GetContainer(0)->GetSize().x;

		listAreaPtr->RemoveContainer(0);
		EXPECT_FALSE(scrollBarPtr->HasStateFlag(StateFlag::Active));
		auto curSizeX = listAreaPtr->GetContainer(0)->GetSize().x;
		EXPECT_NE(preSizeX, curSizeX);

		auto renderTexturePtr = UIEx(listAreaPtr).FindComponent<RenderTexture*>("RenderTexture_0");
		EXPECT_TRUE(listAreaPtr->ChangeSize({ 150, 60 }));
		EXPECT_EQ(renderTexturePtr->GetSize(), XMUINT2(150, 60));
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));

		auto scrollContainerPtr = UIEx(listAreaPtr).FindComponent<Container*>("Container_0");
		EXPECT_EQ(scrollContainerPtr->GetSize().y, 27); //(60 - 6) * (60 / 120) 총 slider 길이(-padding)에 보여줄 컨텐츠 비례해서 크기조정값 

		listAreaPtr->ClearContainers();
		EXPECT_FALSE(listAreaPtr->RemoveContainer(0));

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder*)
	{
		EXPECT_EQ(index, 1); //0은 texture 1은 renderTexture이다. 그래서 1이 들어오고
		vector<RECT> expectDest = { { 75, 75, 125, 125 } };
		vector<RECT> expectSource = { { 0, 0, 50, 50 } };
		TestCoordinates(0, dest, source, expectDest, expectSource);//0값 비교하니까 0을 그냥 넣어줌. 
	}

	TEST_F(ComplexComponentTest, RenderTexture)
	{
		auto [button, buttonPtr] = GetPtrs(CreateSampleButton1({ {32, 32}, Origin::LeftTop }));
		auto renderTex = CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(button));
		auto renderTexPtr = renderTex.get();
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestRenderTexture, 1); //core에 렌더코드가 안 돌기 때문에 한번만 들어온다.
		EXPECT_TRUE(WriteReadTest(m_panel));

		auto clone = renderTexPtr->Clone();
		EXPECT_TRUE(m_renderer->LoadComponent(clone.get()));
	}

	////////////////////////////////////////////////////////

	TEST_F(ComplexComponentTest, ScrollBar)
	{
		auto [scrollBar, scrollBarPtr] = GetPtrs(CreateSampleScrollBar({ {16, 200}, Origin::LeftTop }));
		UIEx(m_panel).AttachComponent(move(scrollBar), { 100, 200 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->UpdateScrollView(viewArea, contentSize);
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
	}

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { 
			{ 92, 100, 108, 110 }, { 92, 110, 108, 290 }, { 92, 290, 108, 300 },
			{ 92, 175, 108, 182 }, { 92, 182, 108, 218 }, { 92, 218, 108, 225 } };

		vector<RECT> expectSource{ GetSources(sb, "ScrollTrack3_V") };
		vector<RECT> expectSource2{ GetSources(sb, "ScrollButton3_Normal_V") };
		expectSource.insert(expectSource.end(), expectSource2.begin(), expectSource2.end());
		TestCoordinates(index, dest, source, expectDest, expectSource);
	}

	TEST_F(ComplexComponentTest, ScrollSlider)
	{
		auto [scrollSlider, scrollSliderPtr] = GetPtrs(CreateSampleScrollSlider(DirectionType::Vertical, { { 16, 200 }, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(scrollSlider), { 100, 200 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		testing::MockFunction<void(float)> mockOnScrollChanged;
		scrollSliderPtr->AddScrollChangedCB(mockOnScrollChanged.AsStdFunction());

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollSliderPtr->SetViewContent(viewArea, contentSize);
		scrollSliderPtr->SetPositionRatio(0.5f);
		CallMockRender(TestScrollBar, 6);

		EXPECT_CALL(mockOnScrollChanged, Call(testing::FloatEq(85.f / 150.f))).Times(1);

		MockMouseInput(100, 200, true); //Pressed
		m_panel->ProcessUpdate(m_timer);

		MockMouseInput(110, 210, true); //벗어났지만 Pressed가 되어야한다.
		m_panel->ProcessUpdate(m_timer);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}
}