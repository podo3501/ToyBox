#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/InputManager.h"

namespace UserInterfaceTest
{
	TEST_F(ComplexComponentTest, ListArea)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		m_panel = WriteReadTest(m_panel, listAreaPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

		EXPECT_TRUE((listAreaPtr->GetSize() == XMUINT2{ 48, 48 }));
		listAreaPtr->ChangeSize({ 150, 128 });
		listAreaPtr->ChangeOrigin(Origin::Center);

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
		EXPECT_TRUE(listAreaPtr->ChangeSize({ 150, 64 }));
		EXPECT_EQ(renderTexturePtr->GetSize(), XMUINT2(150, 64));
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));

		auto scrollContainerPtr = UIEx(listAreaPtr).FindComponent<TextureSwitcher*>("TextureSwitcher_0");
		EXPECT_EQ(scrollContainerPtr->GetSize().y, 30); //스크롤 세로로된 버튼 길이. (64 - padding) * (60 / 120) 총 scrollBar 길이(-padding)에 보여줄 컨텐츠 비례해서 크기조정값 

		listAreaPtr->ClearContainers();
		EXPECT_FALSE(listAreaPtr->RemoveContainer(0));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source, const vector<RECT>&)
	{
		EXPECT_EQ(index, 4); //0, 1은 폰트. 2, 3은 texture 4는 renderTexture이다. 그래서 4가 들어오고
		vector<RECT> expectDest = { { 75, 75, 125, 125 } };
		vector<RECT> expectSource = { { 0, 0, 50, 50 } };
		TestCoordinates(2, dest, source, 2, expectDest, expectSource);//값 비교하니까 2을 그냥 넣어줌. 
	}

	TEST_F(ComplexComponentTest, RenderTexture)
	{
		auto switcher = CreateTextureSwitcher(TextureSlice::One, 
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		auto [renderTex, renderTexPtr] = GetPtrs(CreateRenderTexture(move(switcher)));
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		m_panel = WriteReadTest(m_panel, renderTexPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

		EXPECT_TRUE((renderTexPtr->GetSize() == XMUINT2{ 32, 32 }));
		renderTexPtr->ChangeSize({ 50, 50 });
		renderTexPtr->ChangeOrigin(Origin::Center);

		CallMockRender(TestRenderTexture, "", 1); //core에 렌더코드가 안 돌기 때문에 한번만 들어온다.

		auto clone = renderTexPtr->Clone();
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));
	}

	////////////////////////////////////////////////////////

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { 
			{ 92, 100, 108, 110 }, { 92, 110, 108, 290 }, { 92, 290, 108, 300 },
			{ 92, 175, 108, 182 }, { 92, 182, 108, 218 }, { 92, 218, 108, 225 } };

		vector<RECT> expectSource{ GetSources(rb, "ScrollTrack3_V") };
		vector<RECT> expectSource2{ GetSources(rb, "ScrollButton3_V_Normal") };
		expectSource.insert(expectSource.end(), expectSource2.begin(), expectSource2.end());
		TestCoordinates(index, dest, source, 2, expectDest, expectSource);
	}

	TEST_F(ComplexComponentTest, ScrollBar)
	{
		auto [scrollBar, scrollBarPtr] = GetPtrs(CreateSampleScrollBar({}, DirectionType::Vertical));
		UIEx(m_panel).AttachComponent(move(scrollBar), { 100, 200 });
		m_panel = WriteReadTest(m_panel, scrollBarPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		EXPECT_TRUE((scrollBarPtr->GetSize() == XMUINT2{ 16, 48 }));
		scrollBarPtr->ChangeSize({ 16, 200 });
		scrollBarPtr->ChangeOrigin(Origin::Center);

		testing::MockFunction<void(float)> mockOnScrollChanged;
		scrollBarPtr->AddScrollChangedCB(mockOnScrollChanged.AsStdFunction());

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->SetViewContent(viewArea, contentSize);
		scrollBarPtr->SetPositionRatio(0.5f);
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
		CallMockRender(TestScrollBar, 6);

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
		TestMockRender(2, exDest, "ScrollButton3_V_Pressed");

		MockMouseInput(110, 110, true); //영역에는 벗어났지만 holdToKeepPressed 옵션이 있기 때문에 Pressed가 되어야한다.
		TestMockRender(2, exDest, "ScrollButton3_V_Pressed");
	}
}