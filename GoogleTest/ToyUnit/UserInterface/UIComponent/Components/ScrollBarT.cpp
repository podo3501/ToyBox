#include "pch.h"
#include "ScrollBarT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/StepTimer.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(ScrollBarT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ScrollBarT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 20, 30 })); //자식들의 크기에서 초기값을 가져온다.
	}

	TEST_F(ScrollBarT, OnWheel)
	{
		m_component->SetEnableWheel(true);
		m_component->UpdateScrollView(50, 200);
		m_component->SetPositionRatio(0.5f);

		testing::MockFunction<void(float)> OnScrollChanged;
		m_component->AddScrollChangedCB(OnScrollChanged.AsStdFunction());
		EXPECT_CALL(OnScrollChanged, Call(testing::Gt(0.5))).Times(1);

		m_component->OnWheel(10);

		DX::StepTimer timer;
		this_thread::sleep_for(std::chrono::milliseconds(1));
		timer.Tick([] {});
		m_component->ProcessUpdate(timer);
	}

	TEST_F(ScrollBarT, ProcessRender)
	{
		m_component->ChangeSize({ 20, 100 });

		//화면에 그려지는 부분
		RECT destTrackBottom{ 0, 95, 20, 100 };
		RECT destButtonBottom{ 0, 46, 20, 50 };

		//텍스쳐에서 가져오는 영역
		RECT srcTrackBottom{ 0, 45, 20, 50 };
		RECT srcButtonBottom{ 0, 16, 20, 20 };

		//총 6번을 Render 호출하지만 그중에 마지막 Bottom만을 검사한다.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destTrackBottom, ::testing::Pointee(srcTrackBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destButtonBottom, ::testing::Pointee(srcButtonBottom))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(ScrollBarT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ScrollBarT_WR.json",
			GetResBinder(), GetTextureController()));
	}
}