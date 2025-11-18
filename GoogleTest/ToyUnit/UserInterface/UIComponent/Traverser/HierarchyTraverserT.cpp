#include "pch.h"
#include "HierarchyTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

class MockRenderTraversal : public UIComponentStub
{
public:
	explicit MockRenderTraversal(RenderTraversal traversal = RenderTraversal::Inherited)
	{
		SetRenderTraversal(traversal);
	}

	MOCK_METHOD(void, ImplementRender, (ITextureRender*), (const));
};

class MockRenderBFS : public MockRenderTraversal
{
public:
	MockRenderBFS() : MockRenderTraversal(RenderTraversal::BFS) {}
};

class MockRenderDFS : public MockRenderTraversal
{
public:
	MockRenderDFS() : MockRenderTraversal(RenderTraversal::DFS) {}
};

// node0
//¦§¦¡ node1
//¦¢    ¦¦¦¡ node2
//¦¦¦¡ node3

using namespace UITraverser;

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(HierarchyTraverserT, ForEachChildToRender_BFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderBFS>();
		auto node1 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponent<MockRenderTraversal>(node1);

		vector<UIComponent*> visitOrder;
		auto Func = [&](UIComponent* c) -> TraverseResult {
			visitOrder.push_back(c);
			return TraverseResult::Continue;
			};

		m_traverser.ForEachChildToRender(node0, Func);

		EXPECT_THAT(visitOrder, testing::ElementsAre(node0, node1, node3, node2));
	}

	TEST_F(HierarchyTraverserT, ForEachChildToRender_DFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderDFS>();
		auto node1 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponent<MockRenderTraversal>(node1);

		vector<UIComponent*> visitOrder;
		auto Func = [&](UIComponent* c) -> TraverseResult {
			visitOrder.push_back(c);
			return TraverseResult::Continue;
			};

		m_traverser.ForEachChildToRender(node0, Func);

		EXPECT_THAT(visitOrder, testing::ElementsAre(node0, node1, node2, node3));
	}
}