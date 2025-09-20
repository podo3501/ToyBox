#include "pch.h"
#include "Locator.h"

namespace b_CodeLab
{
    class NodeTraversalProxy;

    class Node {
    private:
        int value;
        std::vector<std::shared_ptr<Node>> children;
        //children�� �ٸ� Ŭ������ �����丵�� �ȵǴ� �����̴�. ���� �� children�� �����ͼ� �Լ� ���� �κ��� �����
        //Helper Ŭ������ �ʿ�������.
        friend class NodeHelper; //friend�� �� ������ GetChildren�Լ��� ���� ������ ���� �����̴�.

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

    ///////////////////////////////////////////////////////////////////////

	class Audio
	{
	public:
		Audio() {}
		~Audio() {}
	};

	class Budio
	{
	public:
		Budio() {}
		~Budio() {}
	};

	TEST(Pattern, Locator)
	{
		//�̱����� ����� Ŭ����
		//Audio Ŭ������ ���ο��� �����ϰ� Locator�� ������ �ٸ� ������ �����ؼ� ����� �� �ִ�.
		//��, Audio Ŭ������ ���ο��� �ѹ��� �����ϰ� Locator Ŭ������ �ٸ� cpp���� ��� �� �� �ֵ���
		//static���� �����Ǿ� �ִ�. �ٸ� Ŭ������ ����ϱ� ���ؼ� template�� �������.
		{
			auto audio = make_unique<Audio>();
			Locator<Audio>::Provide(audio.get());
			const Audio* curAudio = Locator<Audio>::GetService();
		}

		{
			auto budio = make_unique<Budio>();
			Locator<Budio>::Provide(budio.get());
			const Budio* curBudio = Locator<Budio>::GetService();
		}
	}
}