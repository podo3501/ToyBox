#include "pch.h"
#include "SDL3_AudioManager.h"

namespace ThirdParty
{
	TEST_F(SDL3_AudioManager, LoadSound_Ogg)
	{
		EXPECT_TRUE(m_audioManager->LoadSound(GetOggFilename(), AudioGroupID::BGM));
	}

	TEST_F(SDL3_AudioManager, LoadSound_Wav)
	{
		EXPECT_TRUE(m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX));
	}

	TEST_F(SDL3_AudioManager, Unload)
	{
		m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX);
		EXPECT_TRUE(m_audioManager->Unload(GetWavFilename()));
	}

	TEST_F(SDL3_AudioManager, Play)
	{
		m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX);
		m_audioManager->Play(GetWavFilename());

		EXPECT_EQ(m_audioManager->GetPlayState(GetWavFilename()), PlayState::Playing);
	}

	TEST_F(SDL3_AudioManager, Playing)
	{
		m_audioManager->LoadSound(GetWavFilename(), AudioGroupID::SFX);
		m_audioManager->Play(GetWavFilename());
		
		while (m_audioManager->GetPlayState(GetWavFilename()) == PlayState::Playing)
			m_audioManager->Update();
	}
	
	TEST_F(SDL3_AudioManager, Playing_Sounds)
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
}