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

namespace ComponentTest
{
	class MockScrollBar : public ScrollBar
	{
	public:
		MOCK_METHOD(void, ChangeSize, (const XMUINT2& size), (noexcept));
	};

	static unique_ptr<MockScrollBar> CreateSampleMockScrollBar(DirectionType dirType, const UILayout& layout)
	{
		UILayout gridLayout({ layout.GetSize(), Origin::LeftTop });

		auto mockScrollBar = make_unique<MockScrollBar>();
		return mockScrollBar->Setup(layout, CreateScrollTrack(dirType, gridLayout), CreateScrollContainer(gridLayout)) ? move(mockScrollBar) : nullptr;
	}

	TEST_F(ComplexComponentTest, ListArea)
	{
		using ::testing::_;

		UILayout layout{ { 150, 130 }, Origin::Center };
		UILayout scrollBarLayout({ {16, layout.GetSize().y }, Origin::LeftTop });
		auto mockScrollBar = CreateSampleMockScrollBar(DirectionType::Vertical, scrollBarLayout);

		EXPECT_CALL(*mockScrollBar, ChangeSize(_)).Times(1);

		auto container = CreateSampleListContainer(layout);
		auto listArea = CreateListArea(layout, CreateListBackgroudImage(layout), move(container), move(mockScrollBar));
		auto listAreaPtr = ComponentCast<ListArea*>(listArea.get());
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), listAreaPtr, 3));
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
		auto button = CreateSampleButton1({ {32, 32}, Origin::LeftTop });
		auto buttonPtr = button.get();
		auto renderTex = CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(button));
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestRenderTexture, 1); //core�� �����ڵ尡 �� ���� ������ �ѹ��� ���´�.
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
		scrollBarPtr->SetViewContentRatio(static_cast<float>(viewArea) / static_cast<float>(contentSize));
		scrollBarPtr->SetPositionRatio(0.5f);
		CallMockRender(TestScrollBar, 6);

		EXPECT_CALL(mockOnScrollChanged, Call(testing::FloatEq(85.f / 150.f))).Times(1);

		MockMouseInput(100, 200, true); //Pressed
		m_panel->ProcessUpdate(m_timer);

		MockMouseInput(110, 210, true); //������� Pressed�� �Ǿ���Ѵ�.
		m_panel->ProcessUpdate(m_timer);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}
}