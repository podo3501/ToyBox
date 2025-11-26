#include "pch.h"
#include "SDL3_Audio.h"

namespace ThirdParty
{
	TEST_F(SDL3_Audio, LoadWav)
	{
		EXPECT_TRUE(m_audioManager->LoadWav(GetWavFilename(), AudioGroupID::SFX));
	}

	TEST_F(SDL3_Audio, Unload)
	{
		m_audioManager->LoadWav(GetWavFilename(), AudioGroupID::SFX);
		EXPECT_TRUE(m_audioManager->Unload(GetWavFilename()));
	}

	TEST_F(SDL3_Audio, Play)
	{
		m_audioManager->LoadWav(GetWavFilename(), AudioGroupID::SFX);
		m_audioManager->Play(GetWavFilename());

		EXPECT_EQ(m_audioManager->GetPlayState(GetWavFilename()), PlayState::Playing);
	}

	TEST_F(SDL3_Audio, Playing)
	{
		m_audioManager->LoadWav(GetWavFilename(), AudioGroupID::SFX);
		m_audioManager->Play(GetWavFilename());
		
		while (m_audioManager->GetPlayState(GetWavFilename()) == PlayState::Playing)
			m_audioManager->Update();
	}

	TEST_F(SDL3_Audio, Playing_Sounds)
	{
		string sound1 = "../Resources/Sound/Wav/test1.wav";
		string sound2 = "../Resources/Sound/Wav/test2.wav";
		m_audioManager->LoadWav(sound1, AudioGroupID::SFX);
		m_audioManager->LoadWav(sound2, AudioGroupID::SFX);
		m_audioManager->Play(sound2);
		m_audioManager->Play(sound1);

		while (m_audioManager->GetPlayState(sound1) == PlayState::Playing)
			m_audioManager->Update();
	}
}