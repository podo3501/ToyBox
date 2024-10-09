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
}
