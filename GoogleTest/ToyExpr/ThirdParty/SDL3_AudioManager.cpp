#include "pch.h"
#include "SDL3_AudioManager.h"

namespace ThirdParty
{
	TEST_F(SDL3_AudioManager, LoadSound_Ogg)
	{
		//EXPECT_TRUE(m_audioManager->LoadSound(GetOggFilename(), AudioGroupID::BGM));
		EXPECT_TRUE(m_audioManager->LoadSound("TestOgg_01", AudioGroupID::BGM));
	}

	TEST_F(SDL3_AudioManager, LoadSound_Wav)
	{
		EXPECT_TRUE(m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX));
	}

	TEST_F(SDL3_AudioManager, Unload_Ogg)
	{
		m_audioManager->LoadSound(GetOggFilename(), AudioGroupID::BGM);
		EXPECT_TRUE(m_audioManager->Unload(GetOggFilename()));
	}

	TEST_F(SDL3_AudioManager, Unload_Wav)
	{
		m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX);
		EXPECT_TRUE(m_audioManager->Unload(GetWavFilename()));
	}

	TEST_F(SDL3_AudioManager, Play_Ogg)
	{
		m_audioManager->LoadSound(GetOggFilename(), AudioGroupID::BGM);
		m_audioManager->Play(GetOggFilename());

		while (m_audioManager->GetPlayState(GetOggFilename()) == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, Play_Wav)
	{
		m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX);
		m_audioManager->Play(GetWavFilename());
		
		while (m_audioManager->GetPlayState(GetWavFilename()) == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, Play_OggSounds)
	{
		string sound1 = "../Resources/Sound/Ogg/mouseclick1.ogg";
		string sound2 = "../Resources/Sound/Ogg/snare_acoustic01.ogg";
		m_audioManager->LoadSound(sound1, AudioGroupID::BGM);
		m_audioManager->LoadSound(sound2, AudioGroupID::BGM);
		m_audioManager->Play(sound2);
		m_audioManager->Play(sound1);

		while (m_audioManager->GetPlayState(sound1) == PlayState::Playing)
			m_audioManager->Update();
	}
	
	TEST_F(SDL3_AudioManager, Play_WavSounds)
	{
		string sound1 = "../Resources/Sound/Wav/test1.wav";
		string sound2 = "../Resources/Sound/Wav/test2.wav";
		m_audioManager->LoadSound(sound1, AudioGroupID::SFX);
		m_audioManager->LoadSound(sound2, AudioGroupID::SFX);
		m_audioManager->Play(sound2);
		m_audioManager->Play(sound1);

		while (m_audioManager->GetPlayState(sound1) == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_AudioManager, Play_Sounds)
	{
		string sound1 = "../Resources/Sound/Wav/test1.wav";
		string sound2 = "../Resources/Sound/Wav/test2.wav";
		string sound3 = "../Resources/Sound/Ogg/hope_it_will.ogg";
		m_audioManager->LoadSound(sound1, AudioGroupID::SFX);
		m_audioManager->LoadSound(sound2, AudioGroupID::SFX);
		m_audioManager->LoadSound(sound3, AudioGroupID::BGM);
		m_audioManager->Play(sound1);
		m_audioManager->Play(sound2);
		m_audioManager->Play(sound3);

		while (m_audioManager->GetPlayState(sound1) == PlayState::Playing)
			m_audioManager->Update();
	}
}