#include "pch.h"
#include "BaseTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/Input/IMouseEventReceiver.h"

using namespace UITraverser;

class MockReceiverComponenT : public UIComponentStub, public IMouseEventReceiver
{
public:
	virtual IMouseEventReceiver* AsMouseEventReceiver() noexcept override { return this; }
	bool Setup() noexcept { return true; } //CreateComponent 할때 필요한 함수.
};

namespace D::UserInterface::UIComponent::Traverser
{
	TEST_F(BaseTraverserT, GetChildrenBoundsSize)
	{
		auto main = CreateComponent<MockComponent>();
		ChangeSize(main.get(), 10, 10);
		ComponentDesc desc(XMINT2{ 5, 5 }, UILayout({ 10, 10 })); //크기는 10, 10, 위치는 5, 5 에 attach 한다.
		auto component = AttachMockComponent<MockComponent>(main.get(), desc);

		EXPECT_EQ(m_traverser.GetChildrenBoundsSize(main.get()), XMUINT2(15, 15));
	}

	TEST_F(BaseTraverserT, PickComponents)
	{
		UILayout childLayout{ {100, 100}, Origin::LeftTop };
		UILayout parentLayout{ {50, 50}, Origin::LeftTop };
		auto [owner, child] = CreateMockComponent<MockComponent>(childLayout);
		auto parent = CreateComponent<RenderTexture>(parentLayout, move(owner));
		UpdatePositionsManually(parent.get());

		EXPECT_EQ(m_traverser.PickComponents(parent.get(), {45, 45}).size(), 2); //RenderTexture 안쪽이니까 2개가 있어야 한다.
		EXPECT_EQ(m_traverser.PickComponents(parent.get(), { 65, 65 }).size(), 0); //RenderTexture 바깥이니까 아무것도 없어야 한다.
	}

	TEST_F(BaseTraverserT, PickMouseReceivers)
	{
		UILayout childLayout{ {100, 100}, Origin::LeftTop };
		UILayout parentLayout{ {50, 50}, Origin::LeftTop };
		auto [owner, child] = CreateMockComponent<MockReceiverComponenT>(childLayout);
		auto parent = CreateComponent<RenderTexture>(parentLayout, move(owner));
		UpdatePositionsManually(parent.get());

		EXPECT_EQ(m_traverser.PickMouseReceivers(parent.get(), { 45, 45 }).size(), 1); //RenderTexture 안쪽이니까 1개가 있어야 한다.
		EXPECT_EQ(m_traverser.PickMouseReceivers(parent.get(), { 65, 65 }).size(), 0); //RenderTexture 바깥이니까 아무것도 없어야 한다.
	}
}

//Attachment 이 Traverser를 지우고 이걸 NameTraverser로 이동. 그리고 나머지 애들은 적절한 Traverser 이름을
//지어서 그쪽으로 이동. 