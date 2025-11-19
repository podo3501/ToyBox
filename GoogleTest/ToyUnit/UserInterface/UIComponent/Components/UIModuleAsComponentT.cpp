#include "pch.h"
#include "UIModuleAsComponentT.h"
#include "../MockComponent.h"
#include "Internal/MockInputManager.h"
#include "Toy/Locator/InputLocator.h"
#include "Toy/UserInterface/Input/IMouseEventReceiver.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Shared/System/StepTimer.h"

class MockReceiverComponent : public UIComponentStub, public IMouseEventReceiver
{
public:
	virtual IMouseEventReceiver* AsMouseEventReceiver() noexcept override { return this; }
	MOCK_METHOD(void, OnHover, (), (noexcept));
	MOCK_METHOD(void, Render, (ITextureRender*), (const));
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

	TEST_F(UIModuleAsComponentT, Render)
	{
		//모듈에 컴포넌트 하나를 붙인다.
		auto module = m_component->GetUIModule();
		auto mainPanel = module->GetMainPanel();
		auto child = AttachMockComponent<MockReceiverComponent>(mainPanel);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*child, Render(testing::_)).Times(1);

		MockTextureRender render;
		UITraverser::Render(m_component.get(), &render);
	}

	TEST_F(UIModuleAsComponentT, ImplementUpdate)
	{
		//모듈에 컴포넌트 하나를 붙인다.
		auto module = m_component->GetUIModule();
		auto mainPanel = module->GetMainPanel();
		auto child = AttachMockComponent<MockReceiverComponent>(mainPanel);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*child, OnHover()).Times(1);

		//마우스 셋팅 및 위치시키기 
		//?!? Helper를 쓰면 마우스나 이런거 될꺼 같다.
		auto input = make_unique<MockInputManager>();
		InputLocator::Provide(input.get());
		input->SetMouseState(child->GetLeftTop(), InputState::Up);

		DX::StepTimer timer;
		m_component->ProcessUpdate(timer);
	}

	//UIModule에 위치값을 셋팅하는거 만들어서 그 위치에서 render 되는지 확인해야한다.
}