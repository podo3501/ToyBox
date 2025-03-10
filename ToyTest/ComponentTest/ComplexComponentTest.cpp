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
		auto [scrollContainer, scrollContainerPtr] = GetPtrs(CreateScrollContainer({ {16, 100}, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(scrollContainer), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		testing::MockFunction<void(KeyState)> mockOnPress;
		scrollContainerPtr->AddPressCB(mockOnPress.AsStdFunction());

		EXPECT_CALL(mockOnPress, Call(KeyState::Pressed)).Times(1); //Pressed ���ڸ� �־ �ѹ� ȣ���� ���� ���
		EXPECT_CALL(mockOnPress, Call(KeyState::Held)).Times(1);

		MockMouseInput(100, 100, true); //Pressed
		CallMockRender(TestContainer_Scroll, 3);

		MockMouseInput(110, 110, true); //�������� ������� holdToKeepPressed �ɼ��� �ֱ� ������ Held�� �Ǿ���Ѵ�.
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
		EXPECT_EQ(scrollContainerPtr->GetSize().y, 27); //(60 - 6) * (60 / 120) �� slider ����(-padding)�� ������ ������ ����ؼ� ũ�������� 

		listAreaPtr->ClearContainers();
		EXPECT_FALSE(listAreaPtr->RemoveContainer(0));

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_EQ(index, 1); //0�� texture 1�� renderTexture�̴�. �׷��� 1�� ������
		TestRender(0, dest, source, {//0�� ���ϴϱ� 0�� �׳� �־���. 
			{{75, 75, 125, 125}, {0, 0, 50, 50}}
			});
	}

	TEST_F(ComplexComponentTest, RenderTexture)
	{
		auto [button, buttonPtr] = GetPtrs(CreateSampleButton1({ {32, 32}, Origin::LeftTop }));
		auto renderTex = CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(button));
		auto renderTexPtr = renderTex.get();
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestRenderTexture, 1); //core�� �����ڵ尡 �� ���� ������ �ѹ��� ���´�.
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

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			//{ {92, 100, 108, 300 }, {118, 138, 150, 170} },
			{ {92, 100, 108, 110 }, {114, 178, 130, 188} },
			{ {92, 110, 108, 290 }, {114, 188, 130, 216} },
			{ {92, 290, 108, 300 }, {114, 216, 130, 226} },
			{ {92, 175, 108, 182 }, {134, 178, 150, 185} },
			{ {92, 182, 108, 218 }, {134, 185, 150, 187} },
			{ {92, 218, 108, 225 }, {134, 187, 150, 194} }
			});
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

		MockMouseInput(110, 210, true); //������� Pressed�� �Ǿ���Ѵ�.
		m_panel->ProcessUpdate(m_timer);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}
}