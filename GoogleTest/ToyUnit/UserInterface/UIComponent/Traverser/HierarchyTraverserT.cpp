#include "pch.h"
#include "HierarchyTraverserT.h"
#include "../MockComponent.h"

class MockRenderTraversal : public UIComponentStub
{
public:
	explicit MockRenderTraversal(RenderTraversal traversal = RenderTraversal::Inherited)
	{
		SetRenderTraversal(traversal);
	}

	MOCK_METHOD(void, Render, (ITextureRender*), (const));
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

namespace D::UserInterface::UIComponent::Traverser
{
	TEST_F(HierarchyTraverserT, ForEachChildToRender_BFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderBFS>();
		auto node1 = AttachMockComponentDirect<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponentDirect<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponentDirect<MockRenderTraversal>(node1);

		vector<::UIComponent*> visitOrder;
		auto Func = [&](::UIComponent* c) -> TraverseResult {
			visitOrder.push_back(c);
			return TraverseResult::Continue;
			};

		m_traverser.ForEachChildToRender(node0, Func);

		EXPECT_THAT(visitOrder, testing::ElementsAre(node0, node1, node3, node2));
	}

	TEST_F(HierarchyTraverserT, ForEachChildToRender_DFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderDFS>();
		auto node1 = AttachMockComponentDirect<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponentDirect<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponentDirect<MockRenderTraversal>(node1);

		vector<::UIComponent*> visitOrder;
		auto Func = [&](::UIComponent* c) -> TraverseResult {
			visitOrder.push_back(c);
			return TraverseResult::Continue;
			};

		m_traverser.ForEachChildToRender(node0, Func);

		EXPECT_THAT(visitOrder, testing::ElementsAre(node0, node1, node2, node3));
	}
}