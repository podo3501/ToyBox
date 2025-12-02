#include "pch.h"
#include "SDLAudioManager.h"
#include "ISoundTableReader.h"
#include "EffectSound.h"
#include "NormalSound.h"
#include "SDL3/SDL_init.h"

struct AudioGroup
{
	float volume{ 1.f };
};

SDLAudioManager::~SDLAudioManager() 
{ 
	m_normalSound.reset();
	m_effectSound.reset();
	m_reader.reset();
	SDL_Quit();
}

SDLAudioManager::SDLAudioManager(unique_ptr<ISoundTableReader> soundReader) :
	m_reader{ move(soundReader) },
	m_effectSound{ make_unique<EffectSound>() },
	m_normalSound{ make_unique<NormalSound>() }
{}

bool SDLAudioManager::Initialize()
{
	bool isInit = SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO;
	if (isInit) return true;

	ReturnIfFalse(SDL_Init(SDL_INIT_AUDIO));
	ReturnIfFalse(m_effectSound->Initialize());
	ReturnIfFalse(m_normalSound->Initialize());
	CreateAudioGroup();
	return true;
}

void SDLAudioManager::CreateAudioGroup() noexcept
{
	for (int id : views::iota(0, static_cast<int>(AudioGroupID::None)))
		m_audioGroups[static_cast<AudioGroupID>(id)] = make_unique<AudioGroup>();
}

static bool IsWav(const string& filename)
{
	string lower = filename;
	ranges::transform(lower, lower.begin(), [](unsigned char c) { return tolower(c); });

	return lower.ends_with(".wav");
}

bool SDLAudioManager::LoadSound(const string& filename, AudioGroupID groupID)
{
	if (IsWav(filename))
		return m_effectSound->LoadWav(filename, groupID, GetVolume(groupID));
	else
		return m_normalSound->LoadSound(filename, groupID, GetVolume(groupID));
}

bool SDLAudioManager::LoadSound(const string& index)
{
	auto info = m_reader->GetInfo(index);
	if (!info) return false;

	const auto& filename = info->filename;
	auto groupID = info->groupID;
	auto volume = GetVolume(groupID);

	if (IsWav(filename))
		return m_effectSound->LoadWav(filename, groupID, volume);
	else
		return m_normalSound->LoadSound(filename, groupID, volume);
}

bool SDLAudioManager::Unload(const string& filename) noexcept
{
	bool esUnload = m_effectSound->Unload(filename);
	if (esUnload) return true;

	return m_normalSound->Unload(filename);
}

void SDLAudioManager::SetVolume(AudioGroupID groupID, float volume) noexcept
{
	auto& audioGroup = m_audioGroups[groupID];
	audioGroup->volume = volume;

	m_effectSound->SetVolume(groupID, volume);
	m_normalSound->SetVolume(groupID, volume);
}

float SDLAudioManager::GetVolume(AudioGroupID groupID) const noexcept
{
	float masterVolume = m_audioGroups.at(AudioGroupID::Master)->volume;
	float groupVolume = m_audioGroups.at(groupID)->volume;
	float volume = masterVolume * groupVolume;

	return std::clamp(volume, 0.f, 1.f);
}

bool SDLAudioManager::Play(const string& filename)
{
	bool esPlay = m_effectSound->Play(filename);
	if (esPlay) return true;

	return m_normalSound->Play(filename);
}

PlayState SDLAudioManager::GetPlayState(const string& filename)
{
	PlayState esState = m_effectSound->GetPlayState(filename);
	if (esState != PlayState::NotLoaded) return esState;

	return m_normalSound->GetPlayState(filename);
}

void SDLAudioManager::Update() noexcept
{
	m_effectSound->Update();
}

unique_ptr<IAudioManager> CreateAudioManager(unique_ptr<ISoundTableReader> soundReader)
{
	return make_unique<SDLAudioManager>(move(soundReader));
}