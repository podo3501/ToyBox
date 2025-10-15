#include "pch.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Shared/Utils/Memory.h"

namespace CodeLab
{
	class Derived;
	// CRTP를 사용하여 정적 다형성 구현
	template <typename Derived>
	class Base {
	public:
		void call() {
			// Derived 타입의 메서드를 호출하는 방식
			static_cast<Derived*>(this)->hello();
		}
	};

	// Base를 상속받은 클래스
	class Derived : public Base<Derived>
	{
	protected:
		void hello() const {
			std::cout << "Hello from Derived class!" << std::endl;
		}

	private:
		friend class Base<Derived>; // 이제 Base<Derived>는 hello()에 접근 가능
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
			//복사 생성자에서는 move 구문이 먹히지 않는다
			MoveTest& operator=(const MoveTest& other)
			{
				if (this == &other)
					return *this;

				m_data = std::make_unique<UILayout>(*other.m_data);
				//m_data = move(other.m_data); 
				//이렇게 될것 같지만 이 함수가 복사 생성자이기 때문에 move가 안된다.
				//만약 가능하다면 other에 값이 사라지기 때문에(값이 변하기 때문에) other가 'const' 변수라서 여기에 막혀 에러를 뱉어내게 된다.

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
		//2중반복문을 사용할때에는 2중반복문을 일반화 해서 사용하면 편하게 사용할 수 있다.
		//리팩토링을 해야 할 경우가 생기면 조금 번거롭겠지만, 되도록이면 리팩토링이 잘 안될
		//2중반복문에 한해서 사용하는게 좋을 듯 싶다.
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