#pragma once
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/System/Public/IAudioManager.h"
#include "Tests/ToyUnit/Internal/MockSoundTableReader.h"
#include "SDL3/SDL_Init.h"

class SDL3_AudioManager : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;
	string GetWavFilename() const noexcept { return "../Resources/Sound/Wav/click5.wav"; }
	string GetOggFilename() const noexcept { return "../Resources/Sound/Ogg/mouseclick1.ogg"; }

	unique_ptr<Environment> m_environment;
	unique_ptr<IAudioManager> m_audioManager;
};

void SDL3_AudioManager::SetUp()
{
	m_environment = InitializeEnvironment(L"../Resources/", { 800.f, 600.f });

	unique_ptr<MockSoundTableReader> soundReader = make_unique<MockSoundTableReader>();
	soundReader->SetInfo("TestWav_01", { "Sound/Wav/test1.wav", AudioGroupID::SFX, 1.0f });
	soundReader->SetInfo("TestWav_02", { "Sound/Wav/test2.wav", AudioGroupID::SFX, 1.0f });
	soundReader->SetInfo("TestOgg_01", { "Sound/Ogg/mouseclick1.ogg", AudioGroupID::BGM, 1.0f });
	soundReader->SetInfo("TestOgg_02", { "Sound/Ogg/snare_acoustic01.ogg", AudioGroupID::BGM, 1.0f });
	soundReader->SetInfo("TestOgg_03", { "Sound/Ogg/hope_it_will.ogg", AudioGroupID::BGM, 1.0f });

	m_audioManager = CreateAudioManager(move(soundReader));
	m_audioManager->Initialize();
}

void SDL3_AudioManager::TearDown()
{
	m_audioManager.reset();
	SDL_Quit();
}
