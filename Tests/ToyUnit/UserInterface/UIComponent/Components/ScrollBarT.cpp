#include "pch.h"
#include "ScrollBarT.h"

namespace D::UserInterface::UIComponent::Component
{
	TEST_F(ScrollBarT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ScrollBarT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 20, 30 })); //자식들의 크기에서 초기값을 가져온다.
		EXPECT_NEAR(m_button->GetSize().y, 7.5, 0.5); // 30 을 1/4로 나눈값이 대충 7 언저리(50/200)
	}

	TEST_F(ScrollBarT, OnWheel)
	{
		testing::MockFunction<void(float)> OnScrollChanged;
		m_component->AddScrollChangedCB(OnScrollChanged.AsStdFunction());
		EXPECT_CALL(OnScrollChanged, Call(testing::Gt(0.5))).Times(1); //0이 최상단, 1이 최하단.

		m_component->OnWheel(-1); //휠을 내린다. 그럼 0.5보다 큰값이 예상된다.

		DX::StepTimer timer = GetTickTimer();
		Update(m_component.get(), timer);
	}

	TEST_F(ScrollBarT, OnPressCB_MouseDrag)
	{
		testing::MockFunction<void(float)> OnScrollChanged;
		m_component->AddScrollChangedCB(OnScrollChanged.AsStdFunction());
		EXPECT_CALL(OnScrollChanged, Call(testing::Gt(0.5))).Times(1); //0이 최상단, 1이 최하단.

		XMINT2 startPos = m_button->GetLeftTop();
		XMINT2 endPos{ startPos.x, startPos.y + 2 }; //밑으로 y로 2만큼 드래그

		SimulateDrag(m_component.get(), startPos, endPos);
	}

	TEST_F(ScrollBarT, Render)
	{
		ChangeSize(m_component.get(), { 20, 100 });

		//화면에 그려지는 부분
		RECT destTrackBottom{ 0, 95, 20, 100 };
		RECT destButtonBottom{ 0, 55, 20, 59 };

		//텍스쳐에서 가져오는 영역
		RECT srcTrackBottom{ 0, 45, 20, 50 };
		RECT srcButtonBottom{ 0, 16, 20, 20 };

		//총 6번을 Render 호출하지만 그중에 마지막 Bottom만을 검사한다.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destTrackBottom, ::testing::Pointee(srcTrackBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destButtonBottom, ::testing::Pointee(srcButtonBottom))).Times(1);

		UpdatePositionsManually(m_component.get());
		Render(m_component.get(), &render);
	}

	TEST_F(ScrollBarT, RestoreDefault)
	{
		auto newScrollBar = CreateSampleScrollBar({}, DirectionType::Vertical, "Track", "Button");
		BindResource(newScrollBar.get());

		m_component->RestoreDefault(); //초기값으로 셋팅한다.
		EXPECT_TRUE(CompareDerived(m_component, newScrollBar));
	}

	TEST_F(ScrollBarT, WriteAndRead)
	{
		m_component->RestoreDefault(); //처음 셋팅과 같이 만든후 테스트 한다.
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ScrollBarT_WR.json", GetResBinder()));
	}
}