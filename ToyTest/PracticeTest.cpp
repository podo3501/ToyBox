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
}
