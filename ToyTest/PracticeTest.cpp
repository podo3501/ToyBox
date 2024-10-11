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
}
