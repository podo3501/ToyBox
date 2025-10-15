#include "pch.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Shared/Utils/Memory.h"

namespace CodeLab
{
	class Derived;
	// CRTP�� ����Ͽ� ���� ������ ����
	template <typename Derived>
	class Base {
	public:
		void call() {
			// Derived Ÿ���� �޼��带 ȣ���ϴ� ���
			static_cast<Derived*>(this)->hello();
		}
	};

	// Base�� ��ӹ��� Ŭ����
	class Derived : public Base<Derived>
	{
	protected:
		void hello() const {
			std::cout << "Hello from Derived class!" << std::endl;
		}

	private:
		friend class Base<Derived>; // ���� Base<Derived>�� hello()�� ���� ����
	};

	TEST(CPP, CRTP)
	{
		Derived d;
		d.call();
	}

	TEST(CPP, Move)
	{
		class MoveTest
		{
		public:
			virtual ~MoveTest() {};
			//���� �����ڿ����� move ������ ������ �ʴ´�
			MoveTest& operator=(const MoveTest& other)
			{
				if (this == &other)
					return *this;

				m_data = std::make_unique<UILayout>(*other.m_data);
				//m_data = move(other.m_data); 
				//�̷��� �ɰ� ������ �� �Լ��� ���� �������̱� ������ move�� �ȵȴ�.
				//���� �����ϴٸ� other�� ���� ������� ������(���� ���ϱ� ������) other�� 'const' ������ ���⿡ ���� ������ ���� �ȴ�.

				return *this;
			}

			virtual void Test() {}

		private:
			unique_ptr<UILayout> m_data;
		};

		unique_ptr<MoveTest> test1 = make_unique<MoveTest>();
		unique_ptr<MoveTest> test2 = make_unique<MoveTest>();

		test1 = move(test2);
	}

	struct NestedData
	{
		int i{ 1 };
	};

	class NestedForLoops
	{
	public:
		NestedForLoops()
		{
			NestedDatas datas(5);
			m_typeDatas.emplace_back(datas);
			m_typeDatas.emplace_back(datas);
		}

		int Excute()
		{
			int sum{ 0 };
			Each([&sum](const NestedData& data) { sum += data.i; });
			return sum;
		}

	private:
		void Each(std::function<void(const NestedData&)> action)
		{
			for (const auto& curDatas : m_typeDatas)
				for (const auto& curData : curDatas)
					action(curData);
		}

		using NestedDatas = vector<NestedData>;
		vector<NestedDatas> m_typeDatas;
	};


	TEST(CPP, NestedForLoops)
	{
		//2�߹ݺ����� ����Ҷ����� 2�߹ݺ����� �Ϲ�ȭ �ؼ� ����ϸ� ���ϰ� ����� �� �ִ�.
		//�����丵�� �ؾ� �� ��찡 ����� ���� ���ŷӰ�����, �ǵ����̸� �����丵�� �� �ȵ�
		//2�߹ݺ����� ���ؼ� ����ϴ°� ���� �� �ʹ�.
		NestedForLoops loops;
		EXPECT_EQ(loops.Excute(), 10);
	}

	TEST(CPP, RefCounted)
	{
		RefCounted<vector<int>> data;
		data.IncRef();
		EXPECT_TRUE(data.DecRef());
		EXPECT_FALSE(data.DecRef());
		EXPECT_FALSE(data.DecRef());
		EXPECT_EQ(data.GetRefCount(), 0);
	}
}