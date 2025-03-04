#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/Container.h"
#include "../Toy/InputManager.h"

namespace ComponentTest
{
	static void TestContainer_Scroll(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, { //Pressed
			{{92, 50, 108, 57}, {174, 178, 190, 185}},
			{{92, 57, 108, 143}, {174, 185, 190, 187}},
			{{92, 143, 108, 150}, {174, 187, 190, 194}}
			});
	}

	TEST_F(ComplexComponentTest, Container_Scroll)
	{
		auto scrollContainer = CreateScrollContainer({ {16, 100}, Origin::Center });
		auto scrollContainerPtr = scrollContainer.get();
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
		auto listArea = CreateSampleListArea1({ { 150, 130 }, Origin::Center });
		auto listAreaPtr = ComponentCast<ListArea*>(listArea.get());
		auto scrollBarPtr = UIEx(listAreaPtr).FindComponent<ScrollBar*>("ScrollBar_0");
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), listAreaPtr, 7));
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));

		listAreaPtr->ClearContainers();
		//EXPECT_FALSE(scrollBarPtr->HasStateFlag(StateFlag::Active));

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_EQ(index, 1); //0은 texture 1은 renderTexture이다. 그래서 1이 들어오고
		TestRender(0, dest, source, {//0값 비교하니까 0을 그냥 넣어줌. 
			{{75, 75, 125, 125}, {0, 0, 50, 50}}
			});
	}

	TEST_F(ComplexComponentTest, RenderTexture)
	{
		auto button = CreateSampleButton1({ {32, 32}, Origin::LeftTop });
		auto buttonPtr = button.get();
		auto renderTex = CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(button));
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestRenderTexture, 1); //core에 렌더코드가 안 돌기 때문에 한번만 들어온다.
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{ {92, 100, 108, 110 }, {114, 178, 130, 188} },
			{ {92, 110, 108, 290 }, {114, 188, 130, 216} },
			{ {92, 290, 108, 300 }, {114, 216, 130, 226} },
			{ {92, 175, 108, 182 }, {134, 178, 150, 185} },
			{ {92, 182, 108, 218 }, {134, 185, 150, 187} },
			{ {92, 218, 108, 225 }, {134, 187, 150, 194} }
			});
	}

	TEST_F(ComplexComponentTest, ScrollBar)
	{
		auto scrollBar = CreateSampleScrollBar(DirectionType::Vertical, { { 16, 200 }, Origin::Center });
		auto scrollBarPtr = scrollBar.get();
		UIEx(m_panel).AttachComponent(move(scrollBar), { 100, 200 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		testing::MockFunction<void(float)> mockOnScrollChanged;
		scrollBarPtr->AddScrollChangedCB(mockOnScrollChanged.AsStdFunction());

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->SetViewContent(viewArea, contentSize);
		scrollBarPtr->SetPositionRatio(0.5f);
		CallMockRender(TestScrollBar, 6);

		EXPECT_CALL(mockOnScrollChanged, Call(testing::FloatEq(85.f / 150.f))).Times(1);

		MockMouseInput(100, 200, true); //Pressed
		m_panel->ProcessUpdate(m_timer);

		MockMouseInput(110, 210, true); //벗어났지만 Pressed가 되어야한다.
		m_panel->ProcessUpdate(m_timer);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}
}