#include "pch.h"
#include "../Toy/Locator.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/UILayout.h"

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
		//2중반복문을 사용할때에는 2중반복문을 일반화 해서 사용하면 편하게 사용할 수 있다.
		//리팩토링을 해야 할 경우가 생기면 조금 번거롭겠지만, 되도록이면 리팩토링이 잘 안될
		//2중반복문에 한해서 사용하는게 좋을 듯 싶다.
		NestedForLoops loops;
		EXPECT_EQ(loops.Excute(), 10);
	}

	TEST(Parser, Test)
	{
		TextProperty textProperty;
		auto result = Parser(
			L"<Hangle><Red>테스트, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>",
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
}
