#include "pch.h"
#include "Locator.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/UIComponent/UILayout.h"
#include "../Core/Utility.h"

namespace CodeLab
{
	const int IMAGE_HEIGHT = 7;
	const int IMAGE_WIDTH = 7;

	vector<Rectangle> findRectangles(int image[IMAGE_HEIGHT][IMAGE_WIDTH])
	{
		vector<Rectangle> rectangles;
		bool visited[IMAGE_HEIGHT][IMAGE_WIDTH] = { false };

		auto isValid = [&](int x, int y) {
			return x >= 0 && x < IMAGE_WIDTH && y >= 0 && y < IMAGE_HEIGHT;
			};

		for (int y = 0; y < IMAGE_HEIGHT; ++y)
		{
			for (int x = 0; x < IMAGE_WIDTH; ++x)
			{
				if (!visited[y][x] && image[y][x] != 0)
				{
					// BFS 시작
					int minX = x, minY = y, maxX = x, maxY = y;
					queue<pair<int, int>> q;
					q.push({ x, y });
					visited[y][x] = true;

					while (!q.empty())
					{
						auto [cx, cy] = q.front();
						q.pop();

						minX = std::min(minX, cx);
						minY = std::min(minY, cy);
						maxX = std::max(maxX, cx);
						maxY = std::max(maxY, cy);

						// 4방향 탐색
						const int dx[] = { -1, 1, 0, 0 };
						const int dy[] = { 0, 0, -1, 1 };
						for (int d = 0; d < 4; ++d)
						{
							int nx = cx + dx[d], ny = cy + dy[d];
							if (isValid(nx, ny))
							{
								bool& curVisited = visited[ny][nx];
								if (!curVisited && image[ny][nx] != 0)
								{
									curVisited = true;
									q.push({ nx, ny });
								}
							}
						}
					}

					int width = maxX - minX + 1;
					int height = maxY - minY + 1;
					rectangles.emplace_back(minX, minY, width, height);
				}
			}
		}

		return rectangles;
	}

	TEST(CoreUtility, FindRectangles)
	{
		int image[IMAGE_HEIGHT][IMAGE_WIDTH] =
		{
			{0, 0, 0, 0, 0, 1, 2},
			{0, 1, 0, 9, 0, 1, 7},
			{0, 7, 0, 4, 0, 0, 0},
			{0, 7, 6, 1, 2, 2, 0},
			{0, 0, 0, 0, 0, 1, 3},
			{0, 1, 2, 0, 0, 7, 0},
			{1, 4, 8, 0, 0, 1, 0}
		};
		auto rectangles = findRectangles(image);
		EXPECT_EQ(rectangles[0], Rectangle(5, 0, 2, 2));
		EXPECT_EQ(rectangles[1], Rectangle(1, 1, 6, 6));
		EXPECT_EQ(rectangles[2], Rectangle(0, 5, 3, 2));
	}

	TEST(CoreUtility, MergeRectangles)
	{
		vector<Rectangle> rectangles{
			Rectangle{0, 0, 3, 3},
			Rectangle{2, 0, 2, 2},	//Rectangle{0, 0, 3, 3}와 intersect
			Rectangle{5, 0, 2, 2},
			Rectangle{1, 4, 5, 3},
			Rectangle{1, 5, 2, 2}	//Rectangle{1, 4, 5, 3}에 포함
		};
		MergeRectangles(rectangles);
		EXPECT_EQ(rectangles[0], Rectangle(0, 0, 4, 3));
		EXPECT_EQ(rectangles[1], Rectangle(5, 0, 2, 2));
		EXPECT_EQ(rectangles[2], Rectangle(1, 4, 5, 3));
	}

	TEST(CoreUtility, CycleIterator)
	{
		CycleIterator addIter(1, 7);
		for (int a = 1; a != 7; ++a)
			addIter.Increase();
		EXPECT_EQ(addIter.GetCurrent(), 1);

		CycleIterator subIter(-1, 7);
		for (int a = 1; a != 2; ++a)
			subIter.Decrease();
		EXPECT_EQ(subIter.GetCurrent(), 6);
	}

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

	class Audio
	{
	public:
		Audio() {};
		~Audio()
		{
			int a = 1;
		}
	};

	class Budio
	{
	public:
		Budio() {};
		~Budio()
		{
		}
	};

	TEST(Pattern, Locator)
	{
		//싱글톤을 대신할 클래스
		//Audio 클래스는 메인에서 생성하고 Locator에 넣으면 다른 곳에서 접근해서 사용할 수 있다.
		//즉, Audio 클래스는 메인에서 한번만 생성하고 Locator 클래스가 다른 cpp에서 사용 할 수 있도록
		//static으로 구성되어 있다. 다른 클래스도 사용하기 위해서 template로 만들었다.
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
