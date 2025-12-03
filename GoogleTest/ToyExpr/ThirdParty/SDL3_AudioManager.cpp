#include "pch.h"
#include "SDL3_AudioManager.h"

//시간이 오래 걸리는 것들만 DISABLED_를 걸자.
namespace ThirdParty
{
	TEST_F(SDL3_AudioManager, LoadSound_Ogg)
	{
		EXPECT_TRUE(m_audioManager->LoadSound("TestOgg_01"));
	}

	TEST_F(SDL3_AudioManager, LoadSound_Wav)
	{
		EXPECT_TRUE(m_audioManager->LoadSound("TestWav_01"));
	}

	TEST_F(SDL3_AudioManager, DISABLED_Play_Ogg)
	{
		m_audioManager->LoadSound("TestOgg_01");
		m_audioManager->Play("TestOgg_01");

		while (m_audioManager->GetPlayState("TestOgg_01") == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, DISABLED_Play_OggSounds)
	{
		m_audioManager->LoadSound("TestOgg_01");
		m_audioManager->LoadSound("TestOgg_02");
		m_audioManager->Play("TestOgg_01");
		m_audioManager->Play("TestOgg_02");

		while (m_audioManager->GetPlayState("TestOgg_01") == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, DISABLED_Play_Sounds)
	{
		m_audioManager->LoadSound("TestWav_01");
		m_audioManager->LoadSound("TestWav_02");
		m_audioManager->LoadSound("TestOgg_03");
		m_audioManager->Play("TestWav_01");
		m_audioManager->Play("TestWav_02");
		m_audioManager->Play("TestOgg_03");

		while (m_audioManager->GetPlayState("TestWav_01") == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, DISABLED_Play_Wav)
	{
		m_audioManager->LoadSound("TestWav_01");
		m_audioManager->Play("TestWav_01");
		
		while (m_audioManager->GetPlayState("TestWav_01") == PlayState::Playing)
			m_audioManager->Update();
	}
	
	TEST_F(SDL3_AudioManager, DISABLED_Play_WavSounds)
	{
		m_audioManager->LoadSound("TestWav_01");
		m_audioManager->LoadSound("TestWav_02");
		m_audioManager->Play("TestWav_01");
		m_audioManager->Play("TestWav_02");

		while (m_audioManager->GetPlayState("TestWav_01") == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, Unload_Ogg)
	{
		m_audioManager->LoadSound("TestOgg_01");
		EXPECT_TRUE(m_audioManager->Unload("TestOgg_01"));
	}

	TEST_F(SDL3_AudioManager, Unload_Wav)
	{
		m_audioManager->LoadSound("TestWav_01");
		EXPECT_TRUE(m_audioManager->Unload("TestWav_01"));
	}
}