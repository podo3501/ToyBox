#include "pch.h"
#include "AttachmentTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;
namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(AttachmentTraverserT, AttachComponent)
	{
		auto parent = CreateComponent<MockComponent>();
		auto child = CreateComponent<MockComponent>();

		EXPECT_EQ(AttachComponent(parent.get(), move(child)), nullptr); //attach가 잘 되면 nullptr을 리턴, 잘못되면 move한 component를 리턴한다.
	}
}