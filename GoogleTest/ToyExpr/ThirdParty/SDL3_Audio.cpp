#include "pch.h"
#include "SDL3_Audio.h"

namespace ThirdParty
{
	TEST_F(SDL3_Audio, LoadWav)
	{
		EXPECT_TRUE(m_audioManager->LoadWav(GetWavFilename()));
	}

	TEST_F(SDL3_Audio, Unload)
	{
		m_audioManager->LoadWav(GetWavFilename());
		EXPECT_TRUE(m_audioManager->Unload(GetWavFilename()));
	}

	TEST_F(SDL3_Audio, Play)
	{
		m_audioManager->LoadWav(GetWavFilename());
		m_audioManager->Play(GetWavFilename());

		EXPECT_EQ(m_audioManager->GetPlayState(GetWavFilename()), PlayState::Playing);
	}
}