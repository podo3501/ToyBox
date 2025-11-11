#include "pch.h"
#include "UIComponentT.h"
#include "MockComponent.h"
#include "Internal/MockRenderer.h"
#include "../TextureResourceBinder/MockTextureResourceBinder.h"

class MockBehaviorComponenT : public UIComponentStub
{
public:
	MockBehaviorComponenT()
	{
		//기본 리턴이 false 이기 때문에 리턴을 하지 않으면 실패했다고 간주한다.
		ON_CALL(*this, ImplementBindSourceInfo(testing::_, testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, ImplementUpdate(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, ImplementBindSourceInfo, (TextureResourceBinder*, ITextureController*), (noexcept)); //파일을 메모리에서 불러와서 키값과 매칭시키는 함수
	MOCK_METHOD(bool, ImplementUpdate, (const DX::StepTimer&), (noexcept)); //업데이트 하면서 좌표를 계산
	MOCK_METHOD(void, ImplementRender, (ITextureRender*), (const)); //화면에 보여주는 부분
};

namespace UserInterfaceT::UIComponentT
{
	TEST_F(UIComponentT, Attach)
	{
		//attach 할때 유니크한 이름 생성은 하지 않고 붙인다.
		auto parent = CreateComponent<MockComponent>();
		auto child = CreateComponent<MockComponent>();

		parent->Attach(move(child));
		EXPECT_EQ(parent->GetChildren().size(), 1);
	}

	TEST_F(UIComponentT, Detach)
	{
		auto [detach, parent] = m_child->Detach();
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
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentLeftTopChanged |
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LeftTop) //LeftTop 일때 변환
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LockPosOnResize) //StateFlag::LockPosOnResize 일때 변환
	{
		m_parent->SetStateFlag(StateFlag::LockPosOnResize, true);

		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged
		);
	}

	TEST_F(UIComponentT, RenderPipeline) //셋팅부터 렌더링까지
	{
		auto component = AttachMockComponent<MockBehaviorComponenT>(m_main);
		testing::InSequence seq;
		EXPECT_CALL(*component, ImplementBindSourceInfo(testing::_, testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementUpdate(testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementRender(testing::_)).Times(1);

		MockRenderer renderer;
		MockTextureResourceBinder resBinder;
		component->BindTextureSourceInfo( &resBinder, renderer.GetTextureController());

		DX::StepTimer timer{};
		component->ProcessUpdate(timer);

		MockTextureRender render;
		component->ProcessRender(&render);
	}

	TEST_F(UIComponentT, StateFlag_Attach)
	{
		m_main->SetStateFlag(StateFlag::Attach, false);	//attach 불가
		auto [owner, _] = CreateMockComponent<MockComponent>();
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(owner));

		EXPECT_NE(result, nullptr); //attach가 안되면 nullptr 값이 아니다.
	}

	TEST_F(UIComponentT, StateFlag_Detach)
	{
		auto [owner, component] = CreateMockComponent<MockComponent>();
		UIEx(m_main).AttachComponent(move(owner));
		m_main->SetStateFlag(StateFlag::Detach, false);
		auto [detach, detachPtr] = UIEx(component).DetachComponent();

		EXPECT_EQ(detach, nullptr); //detach가 안되면 nullptr이 반환된다.
		EXPECT_EQ(detachPtr, nullptr);
	}

	TEST_F(UIComponentT, StateFlag_SizeLocked)
	{
		m_main->ChangeSize(10, 10);
		m_main->SetStateFlag(StateFlag::X_SizeLocked, true);
		m_main->SetStateFlag(StateFlag::Y_SizeLocked, true);
		m_main->ChangeSize(20, 20);

		EXPECT_EQ(m_main->GetSize(), XMUINT2(10, 10));
	}
}