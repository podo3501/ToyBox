#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollSlider.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/InputManager.h"

namespace UserInterfaceTest
{
	TEST_F(ComplexComponentTest, ListArea)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({ { 150, 120 }, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		m_panel = WriteReadTest(m_panel, listAreaPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));
		auto scrollBarPtr = UIEx(listAreaPtr).FindComponent<ScrollBar*>("ScrollBar_0");
		EXPECT_TRUE(UIEx(m_panel).IsPositionUpdated());

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), m_resBinder.get(), listAreaPtr, 5));
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

		auto scrollContainerPtr = UIEx(listAreaPtr).FindComponent<TextureSwitcher*>("TextureSwitcher_0");
		EXPECT_EQ(scrollContainerPtr->GetSize().y, 27); //(60 - 6) * (60 / 120) 총 slider 길이(-padding)에 보여줄 컨텐츠 비례해서 크기조정값 

		listAreaPtr->ClearContainers();
		EXPECT_FALSE(listAreaPtr->RemoveContainer(0));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source, const vector<RECT>&)
	{
		EXPECT_EQ(index, 4); //0, 1은 폰트. 2, 3은 texture 4는 renderTexture이다. 그래서 4가 들어오고
		vector<RECT> expectDest = { { 75, 75, 125, 125 } };
		vector<RECT> expectSource = { { 0, 0, 50, 50 } };
		TestCoordinates(2, dest, source, expectDest, expectSource);//값 비교하니까 2을 그냥 넣어줌. 
	}

	TEST_F(ComplexComponentTest, RenderTexture)
	{
		auto switcher = CreateTextureSwitcher({ {32, 32}, Origin::Center }, 
			TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		auto [renderTex, renderTexPtr] = GetPtrs(CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(switcher)));
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		m_panel = WriteReadTest(m_panel, renderTexPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

		CallMockRender(TestRenderTexture, "", 1); //core에 렌더코드가 안 돌기 때문에 한번만 들어온다.

		auto clone = renderTexPtr->Clone();
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));
	}

	////////////////////////////////////////////////////////

	TEST_F(ComplexComponentTest, ScrollBar)
	{
		auto [scrollBar, scrollBarPtr] = GetPtrs(CreateSampleScrollBar({ {16, 200}, Origin::LeftTop }));
		UIEx(m_panel).AttachComponent(move(scrollBar), { 100, 200 });
		m_panel = WriteReadTest(m_panel, scrollBarPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->UpdateScrollView(viewArea, contentSize);
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
	}

	static void TestScrollSlide(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { 
			{ 92, 100, 108, 110 }, { 92, 110, 108, 290 }, { 92, 290, 108, 300 },
			{ 92, 175, 108, 182 }, { 92, 182, 108, 218 }, { 92, 218, 108, 225 } };

		vector<RECT> expectSource{ GetSources(rb, "ScrollTrack3_V") };
		vector<RECT> expectSource2{ GetSources(rb, "ScrollButton3_V_Normal") };
		expectSource.insert(expectSource.end(), expectSource2.begin(), expectSource2.end());
		TestCoordinates(index, dest, source, expectDest, expectSource);
	}

	TEST_F(ComplexComponentTest, ScrollSlider)
	{
		auto [scrollSlider, scrollSliderPtr] = GetPtrs(CreateSampleScrollSlider(DirectionType::Vertical, { { 16, 200 }, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(scrollSlider), { 100, 200 });
		m_panel = WriteReadTest(m_panel, scrollSliderPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		testing::MockFunction<void(float)> mockOnScrollChanged;
		scrollSliderPtr->AddScrollChangedCB(mockOnScrollChanged.AsStdFunction());

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollSliderPtr->SetViewContent(viewArea, contentSize);
		scrollSliderPtr->SetPositionRatio(0.5f);
		CallMockRender(TestScrollSlide, 6);

		EXPECT_CALL(mockOnScrollChanged, Call(testing::FloatEq(85.f / 150.f))).Times(1);

		MockMouseInput(100, 200, true); //Pressed
		m_panel->ProcessUpdate(m_timer);

		MockMouseInput(110, 210, true); //벗어났지만 Pressed가 되어야한다.
		m_panel->ProcessUpdate(m_timer);
	}

	////////////////////////////////////////////////////////

	TEST_F(ComplexComponentTest, Switcher_Scroll)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {16, 100}, Origin::Center },
			TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::HoldToKeepPressed));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		testing::MockFunction<void(KeyState)> mockOnPress;
		switcherPtr->AddPressCB(mockOnPress.AsStdFunction());

		EXPECT_CALL(mockOnPress, Call(KeyState::Pressed)).Times(1); //Pressed 인자를 넣어서 한번 호출할 것을 기대
		EXPECT_CALL(mockOnPress, Call(KeyState::Held)).Times(1);

		MockMouseInput(100, 100, true); //Pressed
		vector<RECT> exDest = { { 92, 50, 108, 57 }, { 92, 57, 108, 143 }, { 92, 143, 108, 150 } };
		TestMockRender(exDest, "ScrollButton3_V_Pressed");

		MockMouseInput(110, 110, true); //영역에는 벗어났지만 holdToKeepPressed 옵션이 있기 때문에 Pressed가 되어야한다.
		TestMockRender(exDest, "ScrollButton3_V_Pressed");
	}
}