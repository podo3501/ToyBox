#include "pch.h"
#include "../Toy/Locator.h"
#include "../Toy/UserInterface/UIComponent/UIUtility.h"
#include "../Toy/UserInterface/UIComponent/UILayout.h"
#include "../Core/Utility.h"

namespace Practice
{
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

	TEST(Locator, Test)
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

	struct Data
	{
		int i{ 1 };
	};

	class NestedForLoops
	{
	public:
		NestedForLoops()
		{
			Datas datas(5);
			m_typeDatas.emplace_back(datas);
			m_typeDatas.emplace_back(datas);
		}

		int Excute()
		{
			int sum{ 0 };
			Each([&sum](const Data& data) { sum += data.i; });
			return sum;
		}

	private:
		void Each(std::function<void(const Data&)> action)
		{
			for (const auto& curDatas : m_typeDatas)
				for (const auto& curData : curDatas)
					action(curData);
		}

		using Datas = vector<Data>;
		vector<Datas> m_typeDatas;
	};
	

	TEST(NestedForLoops, Test)
	{
		//2�߹ݺ����� ����Ҷ����� 2�߹ݺ����� �Ϲ�ȭ �ؼ� ����ϸ� ���ϰ� ����� �� �ִ�.
		//�����丵�� �ؾ� �� ��찡 ����� ���� ���ŷӰ�����, �ǵ����̸� �����丵�� �� �ȵ�
		//2�߹ݺ����� ���ؼ� ����ϴ°� ���� �� �ʹ�.
		NestedForLoops loops;
		EXPECT_EQ(loops.Excute(), 10);
	}

	TEST(Parser, Test)
	{
		TextProperty textProperty;
		auto result = Parser(
			L"<Hangle><Red>�׽�Ʈ, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>",
			textProperty);
		EXPECT_TRUE(result);
	}

	using json = nlohmann::json;
	TEST(Json, Test)
	{
		ifstream file(L"./ToyTest/TestResources/JsonTest.json");
		if (!file.is_open())
			return;

		json dataList = json::parse(file);
		for (auto& [uiType, properties] : dataList.items())
		{
			for (auto& [property, data] : properties.items())
			{
				if (property == "Filename")
					auto uiFilename = data;
				if (property == "Position")
				{
					for (const auto& posList : data)
					{
						if (posList.is_array() && posList.size() == 4)
							Rectangle test{ posList[0], posList[1], posList[2], posList[3] };
					}
				}
			}
		}
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
					// BFS ����
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

						// 4���� Ž��
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

	TEST(Algorithm, FindRectangles)
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

	TEST(Algorithm, MergeRectangles)
	{
		vector<Rectangle> rectangles{
			Rectangle{0, 0, 3, 3},
			Rectangle{2, 0, 2, 2},	//Rectangle{0, 0, 3, 3}�� intersect
			Rectangle{5, 0, 2, 2},
			Rectangle{1, 4, 5, 3},
			Rectangle{1, 5, 2, 2}	//Rectangle{1, 4, 5, 3}�� ����
		};
		MergeRectangles(rectangles);
		EXPECT_EQ(rectangles[0], Rectangle( 0, 0, 4, 3 ));
		EXPECT_EQ(rectangles[1], Rectangle( 5, 0, 2, 2 ));
		EXPECT_EQ(rectangles[2], Rectangle( 1, 4, 5, 3 ));
	}
}
