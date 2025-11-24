#include "pch.h"
#include "TextureSwitcherT.h"
#include "../ComponentHelper.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Shared/Utils/GeometryExt.h"

class MockEventDispatcherManager : public IEventDispatcherManager
{
	using Callback = function<void(UIEvent)>;

public:
	MOCK_METHOD(void, Subscribe,
		(const string& region, const string& name, Callback cb),
		(noexcept, override));

	MOCK_METHOD(void, Dispatch,
		(const string& region, const string& name, UIEvent event),
		(noexcept, override));

	MOCK_METHOD(void, Clear, (), (noexcept, override));
};

namespace D::UserInterface::UIComponent::Component
{
	TEST_F(TextureSwitcherT, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "Button64_Normal");
		EXPECT_EQ(m_component->GetBindKey(), "Button64_Normal");
	}

	TEST_F(TextureSwitcherT, ChangeState)
	{
		m_component->ChangeState(InteractState::Hovered);
		EXPECT_EQ(m_component->GetState(), InteractState::Hovered);

		m_component->ChangeState(InteractState::Hovered);
	}

	TEST_F(TextureSwitcherT, ChangeState_ForceChange)
	{
		m_component->ChangeState(InteractState::Pressed);
		m_component->ChangeState(InteractState::Hovered);
		EXPECT_EQ(m_component->GetState(), InteractState::Pressed); //Pressed 에서는 hovered가 들어와도 바뀌지 않는다. hovered는 마우스가 올라가기만 하면 계속 호출하기 때문이다.

		m_component->ChangeState(InteractState::Hovered, true); //강제로 바꾸면 바꿔진다. 이건 Tool에서 사용한다.
		EXPECT_EQ(m_component->GetState(), InteractState::Hovered);
	}

	TEST_F(TextureSwitcherT, ClearInteraction)
	{
		m_component->ChangeState(InteractState::Hovered);
		m_component->ClearInteraction();

		EXPECT_EQ(m_component->GetState(), InteractState::Normal);
	}

	TEST_F(TextureSwitcherT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(TextureSwitcherT, Dispatcher)
	{
		auto eventDispatcher = CreateEventDispatcherManager();
		EventDispatcherLocator::Provide(eventDispatcher.get());

		//이벤트를 등록해서 날라오는지 확인한다.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		eventDispatcher->Subscribe(m_component->GetRegion(), m_component->GetName(), mockCallback.AsStdFunction());

		m_component->OnRelease(true);
	}

	TEST_F(TextureSwitcherT, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		ChangeSize(m_component.get(), { 10, 10 });

		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(TextureSwitcherT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 30, 30 })); //초기셋팅은 Normal source 값을 가지고 와서 Size 크기로 셋팅한다.
	}

	TEST_F(TextureSwitcherT, OnNormal)
	{
		m_component->ChangeState(InteractState::Hovered);
		m_component->OnNormal();

		EXPECT_EQ(m_component->GetState(), InteractState::Normal);
	}

	TEST_F(TextureSwitcherT, OnHover)
	{
		m_component->OnHover();
		EXPECT_EQ(m_component->GetState(), InteractState::Hovered);
	}

	TEST_F(TextureSwitcherT, OnPress)
	{
		m_component->OnPress({});
		EXPECT_EQ(m_component->GetState(), InteractState::Pressed);
	}

	TEST_F(TextureSwitcherT, OnHold)
	{
		m_component->OnHold({}, true);
		EXPECT_EQ(m_component->GetState(), InteractState::Pressed);

		m_component->OnHold({}, false);
		EXPECT_EQ(m_component->GetState(), InteractState::Normal);
	}

	TEST_F(TextureSwitcherT, OnRelease)
	{
		auto dispatcher = make_unique<MockEventDispatcherManager>();
		EventDispatcherLocator::Provide(dispatcher.get()); //디스패쳐 등록해서 활성화 시킴
		m_component->SetName("Switcher");
		EXPECT_CALL(*dispatcher, Dispatch("", "Switcher", UIEvent::Clicked)).Times(1);

		m_component->OnRelease(true);
		EXPECT_EQ(m_component->GetState(), InteractState::Normal);
	}

	TEST_F(TextureSwitcherT, Render)
	{
		RECT dest = RectangleToRect(m_component->GetArea());
		RECT source{ 0, 0, 30, 30 };

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source))).Times(1);

		UITraverser::Render(m_component.get(), &render);
	}

	TEST_F(TextureSwitcherT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"TextureSwitcherT_WR.json", GetResBinder()));
	}
}