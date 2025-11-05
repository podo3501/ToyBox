#include "pch.h"
#include "UIModuleT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Shared/System/StepTimer.h"
#include "Shared/Utils/StlExt.h"

class MockInput : public UIComponentStub
{
public:
	explicit MockInput(InputResult pressResult)
	{
		ON_CALL(*this, OnPress(testing::_)).WillByDefault(testing::Return(pressResult));
		ON_CALL(*this, OnWheel(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(void, OnNormal, (), (noexcept));
	MOCK_METHOD(void, OnHover, (), (noexcept));
	MOCK_METHOD(void, OnMove, (const XMINT2& pos), (noexcept));
	MOCK_METHOD(InputResult, OnPress, (const XMINT2& pos), (noexcept));
	MOCK_METHOD(void, OnHold, (const XMINT2& pos, bool inside), (noexcept));
	MOCK_METHOD(void, OnRelease, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnWheel, (int wheelValue), (noexcept));
};

class MockInputPropagate : public MockInput //Propagate를 리턴
{
public:
	MockInputPropagate() : MockInput(InputResult::Propagate) {}
};

class MockInputConsume : public MockInput //Consumed를 리턴
{
public:
	MockInputConsume() : MockInput(InputResult::Consumed) {}
};

class MockToolMode : public UIComponentStub
{
public:
	MOCK_METHOD(bool, EnterToolMode, (), (noexcept));
	MOCK_METHOD(bool, ExitToolMode, (), (noexcept));
};

class MockBehaviorComponent : public UIComponentStub
{
public:
	MockBehaviorComponent()
	{
		//기본 리턴이 false 이기 때문에 리턴을 하지 않으면 제대로된 동작을 안한다고 생각한다.
		ON_CALL(*this, ImplementBindSourceInfo(testing::_, testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, ImplementUpdate(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, ImplementBindSourceInfo, (TextureResourceBinder*, ITextureController*), (noexcept)); //파일을 메모리에서 불러와서 키값과 매칭시키는 함수
	MOCK_METHOD(bool, ImplementUpdate, (const DX::StepTimer&), (noexcept)); //업데이트 하면서 좌표를 계산
	MOCK_METHOD(void, ImplementRender, (ITextureRender*), (const)); //화면에 보여주는 부분
};

/////////////////////////////////////////////////////////

namespace UserInterfaceT
{
	TEST_F(UIModuleT, Clone)
	{
		auto clone = m_uiModule->Clone();
		EXPECT_TRUE(Compare(m_uiModule, clone));
	}

	TEST_F(UIModuleT, EnableToolMode)
	{
		auto comp = CreateOneLevelComponent<MockToolMode>(m_main);

		//툴 모드를 호출하면 모든 컴포넌트에 ToolMode 함수가 호출
		EXPECT_CALL(*comp, EnterToolMode()).Times(1);
		m_uiModule->EnableToolMode(true); 

		EXPECT_CALL(*comp, ExitToolMode()).Times(1);
		m_uiModule->EnableToolMode(false);
	}

	TEST_F(UIModuleT, RenderPipeline)
	{
		auto component = CreateOneLevelComponent<MockBehaviorComponent>(m_main);
		testing::InSequence seq;
		EXPECT_CALL(*component, ImplementBindSourceInfo(testing::_, testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementUpdate(testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementRender(testing::_)).Times(1);

		m_uiModule->BindTextureResources();

		DX::StepTimer timer{};
		m_uiModule->Update(timer);

		MockTextureRender render;
		m_uiModule->Render(&render);
	}

	TEST_F(UIModuleT, UpdateMouseState_ComponentCaptured) //클릭했을때 캡쳐해서 그 컴포넌트를 위주로 실행한다.
	{
		ComponentDesc desc{ XMINT2{10, 10}, UILayout({ 50, 50 }) };
		auto parent = CreateOneLevelComponent<testing::NiceMock<MockInputConsume>>(m_main, desc);
		auto child = CreateOneLevelComponent<testing::NiceMock<MockInputPropagate>>(parent, desc);

		testing::InSequence seq;
		EXPECT_CALL(*child, OnPress(testing::_)).Times(1);
		EXPECT_CALL(*parent, OnPress(testing::_)).Times(1); //consume이기 때문에 parent가 캡쳐된다.
		EXPECT_CALL(*parent, OnHold(testing::_, false)).Times(1); //캡쳐되어서 parent가 반응한다.
		EXPECT_CALL(*parent, OnRelease(false)).Times(1);

		XMINT2 outsidePos{ 0, 0 };
		SimulateDrag(child->GetLeftTop(), outsidePos);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedInside)
	{
		auto comp = CreateOneLevelComponent<MockInputPropagate>(m_main);

		EXPECT_CALL(*comp, OnHover()).Times(3); //UpdateMouseState 불릴때마다 호출
		EXPECT_CALL(*comp, OnMove(testing::_)).Times(3);
		testing::InSequence seq;
		EXPECT_CALL(*comp, OnPress(testing::_)).Times(1);
		EXPECT_CALL(*comp, OnHold(testing::_, true)).Times(1);
		EXPECT_CALL(*comp, OnRelease(true)).Times(1);

		SimulateClick(comp->GetLeftTop());
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedOutside)
	{
		auto comp = CreateOneLevelComponent<MockInputPropagate>(m_main);

		testing::InSequence seq; //호출순서 검증
		EXPECT_CALL(*comp, OnHover()).Times(1); //안에 있을때만 불리기 때문에 한번
		EXPECT_CALL(*comp, OnMove(testing::_)).Times(1);
		EXPECT_CALL(*comp, OnPress(testing::_)).Times(1);
		EXPECT_CALL(*comp, OnNormal()).Times(1); //영역 밖으로 나오면서 호출을 한번 한다.
		EXPECT_CALL(*comp, OnHold(testing::_, false)).Times(1);
		EXPECT_CALL(*comp, OnRelease(false)).Times(1);

		XMINT2 outsidePos{ 0, 0 };
		SimulateDrag(comp->GetLeftTop(), outsidePos);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseHover)
	{
		//NiceMock을 쓴 이유는 OnMove가 계속 호출된다고 warning을 띄우기 때문이다.
		ComponentDesc desc{ 10, 10, UILayout({ 50, 50 }) };
		auto parent = CreateOneLevelComponent<testing::NiceMock<MockInputPropagate>>(m_main, desc);
		auto child = CreateOneLevelComponent<testing::NiceMock<MockInputPropagate>>(parent, desc);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnHover()).Times(1);

		SimulateMouse(child->GetLeftTop(), InputState::Up);

		//parent로 이동하면 child가 OnNormal이 호출되고 parent는 OnHover가 호출되어야 함.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnNormal()).Times(1);

		SimulateMouse(parent->GetLeftTop(), InputState::Up);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseMove)
	{
		auto comp = CreateOneLevelComponent<testing::NiceMock<MockInputPropagate>>(m_main);
		EXPECT_CALL(*comp, OnMove(testing::_)).Times(1);

		SimulateMouse(comp->GetLeftTop(), InputState::Up);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseWheel)
	{
		XMINT2 outsidePos{ 0, 0 };
		auto comp = CreateOneLevelComponent<testing::NiceMock<MockInputPropagate>>(m_main);
		EXPECT_CALL(*comp, OnWheel(testing::_)).Times(1);

		SimulateMouse(comp->GetLeftTop(), 120); //120은 휠의 기본 단위. 
	}

	TEST_F(UIModuleT, WriteAndRead)
	{
		const wstring filename = GetTempDir() + L"UIModuleT_WR.json";
		EXPECT_TRUE(m_uiModule->Write(filename));

		auto read = CreateUIModule(filename, m_renderer.get());
		EXPECT_TRUE(read->BindTextureResources());

		EXPECT_TRUE(Compare(m_uiModule, read));
	}
}