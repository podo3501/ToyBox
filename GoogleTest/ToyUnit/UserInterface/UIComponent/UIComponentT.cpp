#include "pch.h"
#include "UIComponentT.h"
#include "MockComponent.h"
#include "Internal/MockRenderer.h"
#include "../TextureResourceBinder/MockTextureResourceBinder.h"

class MockBehaviorComponent : public UIComponentStub
{
public:
	MockBehaviorComponent()
	{
		//기본 리턴이 false 이기 때문에 리턴을 하지 않으면 실패했다고 간주한다.
		ON_CALL(*this, BindSourceInfo(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, Update(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, BindSourceInfo, (TextureResourceBinder*), (noexcept)); //파일을 메모리에서 불러와서 키값과 매칭시키는 함수
	MOCK_METHOD(bool, Update, (const DX::StepTimer&), (noexcept)); //업데이트 하면서 좌표를 계산
	MOCK_METHOD(void, Render, (ITextureRender*), (const)); //화면에 보여주는 부분
};

namespace D::UserInterface::UIComponent
{
	TEST_F(UIComponentT, Attach)
	{
		//attach 할때 유니크한 이름 생성은 하지 않고 붙인다.
		auto parent = CreateComponent<MockComponent>();
		auto child = CreateComponent<MockComponent>();

		parent->AttachComponent(move(child));
		EXPECT_EQ(parent->GetChildren().size(), 1);
	}

	TEST_F(UIComponentT, Detach)
	{
		auto [detach, parent] = m_child->DetachComponent();
		EXPECT_TRUE(*detach == *m_child);
		EXPECT_TRUE(*parent == *m_parent); //어디서 떨어졌는지 부모 컴포넌트를 알려준다.
	}

	TEST_F(UIComponentT, ChangeOrigin)
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeOrigin(Origin::Center); },
			ChangeExpect::ParentLeftTopChanged
		);
	}

	TEST_F(UIComponentT, ChangeRelativePosition)
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeRelativePosition({ 100, 100 }); },
			ChangeExpect::ParentLeftTopChanged |
			ChangeExpect::ParentRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_Center)
	{
		m_parent->ChangeOrigin(Origin::Center);

		VerifyTransformChange(
			[this] { ChangeSize(m_parent, { 200, 200 }); },
			ChangeExpect::ParentLeftTopChanged |
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LeftTop) //LeftTop 일때 변환
	{
		VerifyTransformChange(
			[this] { 
				ChangeSize(m_parent, { 200, 200 });
			},
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LockPosOnResize) //StateFlag::LockPosOnResize 일때 변환
	{
		m_parent->SetStateFlag(StateFlag::LockPosOnResize, true);

		VerifyTransformChange(
			[this] { ChangeSize(m_parent, { 200, 200 }); },
			ChangeExpect::ParentSizeChanged
		);
	}

	TEST_F(UIComponentT, GetRoot)
	{
		//EXPECT_EQ(m_child->GetRoot(), m_main); //Attach 할때 Root를 저장시킨다.
	}

	TEST_F(UIComponentT, RenderPipeline) //셋팅부터 렌더링까지
	{
		auto [owner, component] = CreateMockComponent<MockBehaviorComponent>();
		m_main->AttachComponent(move(owner));

		testing::InSequence seq;
		EXPECT_CALL(*component, BindSourceInfo(testing::_)).Times(1);
		EXPECT_CALL(*component, Update(testing::_)).Times(1);
		EXPECT_CALL(*component, Render(testing::_)).Times(1);

		MockRenderer renderer;
		MockTextureResourceBinder resBinder;
		BindTextureSourceInfo(component, &resBinder);

		DX::StepTimer timer{};
		Update(component, timer);

		MockTextureRender render;
		Render(component, &render);
	}

	TEST_F(UIComponentT, StateFlag_Attach)
	{
		m_main->SetStateFlag(StateFlag::Attach, false);	//attach 불가
		auto [owner, _] = CreateMockComponent<MockComponent>();
		unique_ptr<::UIComponent> result = m_main->AttachComponent(move(owner));

		EXPECT_NE(result, nullptr); //attach가 안되면 nullptr 값이 아니다.
	}

	TEST_F(UIComponentT, StateFlag_Detach)
	{
		auto [owner, component] = CreateMockComponent<MockComponent>();
		m_main->AttachComponent(move(owner));
		m_main->SetStateFlag(StateFlag::Detach, false);
		auto [detach, detachPtr] = component->DetachComponent();

		EXPECT_EQ(detach, nullptr); //detach가 안되면 nullptr이 반환된다.
		EXPECT_EQ(detachPtr, nullptr);
	}

	TEST_F(UIComponentT, StateFlag_SizeLocked)
	{
		ChangeSize(m_main, 10, 10);
		m_main->SetStateFlag(StateFlag::X_SizeLocked, true);
		m_main->SetStateFlag(StateFlag::Y_SizeLocked, true);
		ChangeSize(m_main, 20, 20);

		EXPECT_EQ(m_main->GetSize(), XMUINT2(10, 10));
	}
}