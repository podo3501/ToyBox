#include "pch.h"
#include "../Toy/Locator.h"

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
}
