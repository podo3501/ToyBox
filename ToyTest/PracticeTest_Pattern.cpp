#include "pch.h"

namespace Practice
{
    class NodeTraversalProxy;

    class Node {
    private:
        int value; 
        std::vector<std::shared_ptr<Node>> children;
        //children이 다른 클래스로 리팩토링이 안되는 변수이다. 따라서 이 children을 가져와서 함수 동작 부분을 만드는
        //Helper 클래스가 필요해진다.
        friend class NodeHelper; //friend로 한 이유는 GetChildren함수를 만들 이유가 없기 때문이다.

    public:
        explicit Node(int value) : value(value) {}

        void AddChild(std::shared_ptr<Node> child) 
        {
            children.push_back(child);
        }

        int GetValue() const 
        {
            return value;
        }
    };

    class NodeHelper
    {
    private:
        static void Traverse(const Node& node, std::function<void(const Node&)> action)
        {
            action(node);
            for (const auto& child : node.children) {
                Traverse(*child, action);
            }
        }

    public:
        static int GetTotalValue(Node* node)
        {
            int sum = 0;
            Traverse(*node, [&sum](const Node& node) {
                sum += node.GetValue();
                });
            return sum;
        }
    };

    TEST(Pattern, Composite)
    {
        auto root = std::make_shared<Node>(10);
        auto child1 = std::make_shared<Node>(20);
        auto child2 = std::make_shared<Node>(30);
        auto grandchild1 = std::make_shared<Node>(40);
        auto grandchild2 = std::make_shared<Node>(50);

        root->AddChild(child1);
        root->AddChild(child2);
        child1->AddChild(grandchild1);
        child2->AddChild(grandchild2);

        int sum = NodeHelper::GetTotalValue(root.get());
        EXPECT_EQ(sum, 150);
    }
}