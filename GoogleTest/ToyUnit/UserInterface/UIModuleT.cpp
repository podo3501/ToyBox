#include "pch.h"
#include "UIModuleT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Shared/System/StepTimer.h"
#include "Shared/Utils/StlExt.h"

class MockClickable : public UIComponentStub
{
public:
	MockClickable()
	{
		//리턴을 하지 않으면 UpdateMouseState 함수 안에서 컴포넌트를 capture 하지 않아 함수가 동작하지 않음.
		ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnHold(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnRelease(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, OnNormal, (), (noexcept));
	MOCK_METHOD(bool, OnHover, (), (noexcept));
	MOCK_METHOD(bool, OnPress, (), (noexcept));
	MOCK_METHOD(bool, OnHold, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnRelease, (bool inside), (noexcept));
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

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedInside)
	{
		auto comp = CreateOneLevelComponent<MockClickable>(m_main);

		EXPECT_CALL(*comp, OnHover()).Times(3); //UpdateMouseState 불릴때마다 호출
		testing::InSequence seq;
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnHold(true)).Times(1);
		EXPECT_CALL(*comp, OnRelease(true)).Times(1);

		SimulateClick(comp->GetLeftTop());
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedOutside)
	{
		XMINT2 outsidePos{ 0, 0 };
		auto comp = CreateOneLevelComponent<MockClickable>(m_main);

		testing::InSequence seq; //호출순서 검증
		EXPECT_CALL(*comp, OnHover()).Times(1); //안에 있을때만 불리기 때문에 한번
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnNormal()).Times(1); //영역 밖으로 나오면서 호출을 한번 한다.
		EXPECT_CALL(*comp, OnHold(false)).Times(1);
		EXPECT_CALL(*comp, OnRelease(false)).Times(1);

		SimulateClick(comp->GetLeftTop(), outsidePos);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseHover)
	{
		ComponentDesc desc{ 10, 10, UILayout({ 50, 50 }) };
		auto parent = CreateOneLevelComponent<MockClickable>(m_main, desc);		
		auto child = CreateOneLevelComponent<MockClickable>(parent, desc);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnHover()).Times(1);

		SimulateMouse(child->GetLeftTop(), InputState::Up);

		//parent로 이동하면 child가 OnNormal이 호출되고 parent는 OnHover가 호출되어야 함.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnNormal()).Times(1);

		SimulateMouse(parent->GetLeftTop(), InputState::Up);
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