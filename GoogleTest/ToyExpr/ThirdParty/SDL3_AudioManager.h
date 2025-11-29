#pragma once
#include "Shared/System/Public/IAudioManager.h"
#include "SDL3/SDL_Init.h"

class SDL3_AudioManager : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;
	string GetWavFilename() const noexcept { return "../Resources/Sound/Wav/click5.wav"; }
	string GetOggFilename() const noexcept { return "../Resources/Sound/Ogg/mouseclick1.ogg"; }

	unique_ptr<IAudioManager> m_audioManager;
};

void SDL3_AudioManager::SetUp()
{
	m_audioManager = CreateAudioManager();
	m_audioManager->Initialize();
	//m_audioManager->SetVolume(AudioGroupID::Master, 0.f);
}

void SDL3_AudioManager::TearDown()
{
	m_audioManager.reset();
	SDL_Quit();
}

