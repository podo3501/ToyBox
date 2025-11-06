#include "pch.h"
#include "UIHierarchyT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Mocks/MockRenderer.h"

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
//├─ node1
//│    └─ node2
//└─ node3

namespace UserInterfaceT::UIComponentT
{
	TEST_F(UIHierarchyT, ForEachChildToRender_BFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderBFS>();
		auto node1 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponent<MockRenderTraversal>(node1);

		::testing::InSequence seq; // 0, 1, 3, 2 순으로 랜더링 되어야 한다.
		EXPECT_CALL(*node0, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node1, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node3, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node2, ImplementRender(testing::_)).Times(1);

		MockTextureRender render;
		node0->ProcessRender(&render);
	}

	TEST_F(UIHierarchyT, ForEachChildToRender_DFS)
	{
		auto [owner, node0] = CreateMockComponent<MockRenderDFS>();
		auto node1 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node3 = AttachMockComponent<MockRenderTraversal>(node0);
		auto node2 = AttachMockComponent<MockRenderTraversal>(node1);

		::testing::InSequence seq; // 0, 1, 2, 3 순으로 랜더링 되어야 한다.
		EXPECT_CALL(*node0, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node1, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node2, ImplementRender(testing::_)).Times(1);
		EXPECT_CALL(*node3, ImplementRender(testing::_)).Times(1);

		MockTextureRender render;
		node0->ProcessRender(&render);
	}
}