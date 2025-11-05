#include "pch.h"
#include "UIModuleAsComponentT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockInputManager.h"
#include "Mocks/MockUtils.h"
#include "Toy/Locator/InputLocator.h"
#include "Shared/System/StepTimer.h"

class MockComponent : public UIComponentStub
{
public:
	MOCK_METHOD(void, OnHover, (), (noexcept));
	MOCK_METHOD(void, ImplementRender, (ITextureRender*), (const));
};

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(UIModuleAsComponentT, GetUIModule)
	{
		EXPECT_TRUE(m_component->GetUIModule());
	}

	TEST_F(UIModuleAsComponentT, GetSize)
	{
		EXPECT_EQ(m_component->GetSize(), XMUINT2(800, 600)); //uiModule 사이즈를 초기값으로 가진다.
	}

	TEST_F(UIModuleAsComponentT, ImplementRender)
	{
		//모듈에 컴포넌트 하나를 붙인다.
		auto module = m_component->GetUIModule();
		auto mainPanel = module->GetMainPanel();
		auto child = CreateOneLevelComponent<MockComponent>(mainPanel);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*child, ImplementRender(testing::_)).Times(1);

		MockTextureRender render;
		m_component->ProcessRender(&render);
	}

	TEST_F(UIModuleAsComponentT, ImplementUpdate)
	{
		//모듈에 컴포넌트 하나를 붙인다.
		auto module = m_component->GetUIModule();
		auto mainPanel = module->GetMainPanel();
		auto child = CreateOneLevelComponent<MockComponent>(mainPanel);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*child, OnHover()).Times(1);

		//마우스 셋팅 및 위치시키기
		auto input = make_unique<MockInputManager>();
		InputLocator::Provide(input.get());
		input->SetMouseState(child->GetLeftTop(), InputState::Up);

		DX::StepTimer timer;
		m_component->ProcessUpdate(timer);
	}

	

	//UIModule에서 위치값을 셋팅하면 그 위치에서 render 되는지 확인해야한다.
}