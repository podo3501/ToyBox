#include "pch.h"
#include "ListAreaT.h"
#include "../ComponentHelper.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Shared/Utils/GeometryExt.h"

using namespace UITraverser;

void ListAreaT::MakeTestPrototype()
{
	auto prototype = m_component->GetPrototypeContainer();
	auto texture = CreateComponent<PatchTextureStd1>("One32");
	texture->SetName("Test");
	prototype->AttachComponentToCenter(move(texture), {});
}

void ListAreaT::MakeTestData(int itemCount)
{
	MakeTestPrototype();

	for (auto idx : views::iota(0, itemCount))
		m_component->PrepareContainer();
}

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(ListAreaT, ClearContainers)
	{
		MakeTestData(3);
		m_component->ClearContainers();

		EXPECT_EQ(m_component->GetContainerCount(), 0);
	}

	TEST_F(ListAreaT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ListAreaT, GetContainer)
	{
		MakeTestData(1);
		auto container = m_component->GetContainer(0);

		EXPECT_TRUE(FindComponent<PatchTextureStd1*>(container, "Test"));
	}

	TEST_F(ListAreaT, GetContainerCount)
	{
		MakeTestData(3);

		EXPECT_EQ(m_component->GetContainerCount(), 3);
	}

	TEST_F(ListAreaT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 48, 48 })); //자식들의 크기에서 초기값을 가져온다.
	}

	TEST_F(ListAreaT, PrepareContainer) //리스트에 컴포넌트 추가해서 값 넣는 방법
	{
		MakeTestPrototype(); // 여기에 기본이 되는 것을 만든다.

		auto container = m_component->PrepareContainer(); // 컨테이너 하나를 준비하면 프로토타입과 같은 것이 준비된다.
		EXPECT_TRUE(FindComponent<PatchTextureStd1*>(container, "Test"));
	}

	TEST_F(ListAreaT, RemoveContainer)
	{
		MakeTestData(2);
		m_component->RemoveContainer(0);

		EXPECT_EQ(m_component->GetContainerCount(), 1);
	}

	TEST_F(ListAreaT, Render)
	{
		ChangeSize(m_component.get(), { 150, 50 });
		MakeTestData(3); //스크롤 바가 나타난다.

		size_t texIndex = 1; //새로 생성된 텍스쳐라 인덱스가 생성됨.
		//화면에 그려지는 부분
		RECT destRenderTexture{ 0, 0, 150, 50 };
		RECT destTrackBottom{ 128, 43,148, 48 };
		RECT destButtonBottom{ 128,13,148,17 };

		//텍스쳐에서 가져오는 영역
		RECT srcRenderTexture{ 0, 0, 150, 50 };
		RECT srcTrackBottom{ 0, 45, 20, 50 };
		RECT srcButtonBottom{ 0, 16, 20, 20 };

		//총 7번을 Render 호출하지만 RenderTexture와 ScrollBar의 마지막 Bottom 2개해서 3개를 검사한다.
		//데이터 부분은 텍스쳐에 찍은다음 텍스쳐를 출력하기 때문에 새로만든 텍스쳐 한장만 찍힌다.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(texIndex, destRenderTexture, ::testing::Pointee(srcRenderTexture))).Times(1);
		EXPECT_CALL(render, Render(0, destTrackBottom, ::testing::Pointee(srcTrackBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destButtonBottom, ::testing::Pointee(srcButtonBottom))).Times(1);

		UpdatePositionsManually(m_component.get());
		Render(m_component.get(), &render);
	}

	TEST_F(ListAreaT, ScrollBarVisible_ChangeSize)
	{
		MakeTestData(3);
		ChangeSize(m_component.get(), 200, 200); //사이즈가 커지면 스크롤바는 보이지 않게 된다.

		EXPECT_FALSE(m_scrollBar->IsVisible());
	}

	TEST_F(ListAreaT, ScrollBarVisible_InsertData)
	{
		MakeTestData(2); //데이터가 2개일때부터 스크롤바가 보이기 시작한다.
		EXPECT_TRUE(m_scrollBar->IsVisible());

		m_component->RemoveContainer(0);
		EXPECT_FALSE(m_scrollBar->IsVisible());
	}
	
	TEST_F(ListAreaT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ListAreaT_WR.json",
			GetResBinder(), GetTextureController()));
	}
}