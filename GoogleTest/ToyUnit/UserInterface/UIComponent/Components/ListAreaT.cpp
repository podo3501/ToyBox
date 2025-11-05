#include "pch.h"
#include "ListAreaT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "Shared/Utils/GeometryExt.h"

void ListAreaT::MakeTestPrototype()
{
	auto prototype = m_component->GetPrototypeContainer();
	auto texture = CreateComponent<PatchTextureStd1>("One32");
	UIEx(texture).Rename("Test");
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

	TEST_F(ListAreaT, Container_MouseHover)
	{
		MakeTestData(6);
		m_component->ChangeSize({ 150, 50 });
		m_scrollBar->SetPositionRatio(1.f); //?!? Drag를 써서 해야겠다.
		m_component->UpdatePositionsManually();
		//auto container = m_component->GetContainer(0);
		//RenderTexture안에서는 컴포넌트를 찾을 수 없기 때문에 OnMove의 마우스의 위치값으로 
		//컨테이너를 찾아서 OnHover를 호출해야 한다.
		//버튼은 자기 혼자일때도 있지만 스크롤바에 속할때도 있다. 그래서 자신은 캡쳐 대상이지만,
		//이벤트를 부모로 올라가야 할때도 발생한다. 그래서 리턴값을 true, false가 아닌 3가지 이상으로
		//할 필요성이 있고, 이걸 bubbling이라고 부른다. 부모 즉, 스크롤바는 이벤트를 모두 소비하고 끝낸다.
		//그렇게 되면 scrollbar에서 버튼 조작이 가능해지고, 굳이 스크롤바에서 버튼을 찾아서 이동시키고 할
		//필요가 없어진다. 자식은 콜백을 안 써도 되고, 코드가 부모에서 자식 방향으로만 전달된다.
		m_component->OnMove({ 30, 45 });// 제일 밑의 컨테이너로 마우스를 갖다댄다.
		auto container = ComponentCast<TextureSwitcher*>(m_component->GetContainer(5));
		//EXPECT_EQ(container->GetState(), InteractState::Hovered);
	}

	TEST_F(ListAreaT, GetContainer)
	{
		MakeTestData(1);
		auto container = m_component->GetContainer(0);

		EXPECT_TRUE(UIEx(container).FindComponent<PatchTextureStd1*>("Test"));
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
		EXPECT_TRUE(UIEx(container).FindComponent<PatchTextureStd1*>("Test"));
	}

	TEST_F(ListAreaT, RemoveContainer)
	{
		MakeTestData(2);
		m_component->RemoveContainer(0);

		EXPECT_EQ(m_component->GetContainerCount(), 1);
	}

	TEST_F(ListAreaT, ProcessRender)
	{
		m_component->ChangeSize({ 150, 50 });
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

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(ListAreaT, ScrollBarVisible_ChangeSize)
	{
		MakeTestData(3);
		m_component->ChangeSize(200, 200); //사이즈가 커지면 스크롤바는 보이지 않게 된다.

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