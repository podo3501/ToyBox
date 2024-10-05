#include "pch.h"

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

	template<typename T>
	class TLocator
	{
	public:
		static T* GetService() { return m_service; }
		static void Provide(T* service) { m_service = service; }

	private:
		static T* m_service;
	};

	template<typename T>
	T* TLocator<T>::m_service{ nullptr };

	TEST(Locator, Test)
	{
		auto audio = make_unique<Audio>();
		TLocator<Audio>::Provide(audio.get());
		Audio* curAudio = TLocator<Audio>::GetService();

		auto budio = make_unique<Budio>();
		TLocator<Budio>::Provide(budio.get());
		Budio* curBudio = TLocator<Budio>::GetService();
	}
}
