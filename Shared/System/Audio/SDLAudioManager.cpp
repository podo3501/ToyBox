#include "pch.h"
#include "SDLAudioManager.h"
#include "AudioBuffer.h"
#include "SDL3/SDL_init.h"

struct AudioGroup
{
	float volume{ 1.f };
};

SDLAudioManager::~SDLAudioManager() { SDL_QuitSubSystem(SDL_INIT_AUDIO); }
SDLAudioManager::SDLAudioManager() = default;
bool SDLAudioManager::Initialize()
{
	bool isInit = SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO;
	if (isInit) return true;

	ReturnIfFalse(SDL_InitSubSystem(SDL_INIT_AUDIO));
	CreateAudioGroup();
	return true;
}

void SDLAudioManager::CreateAudioGroup() noexcept
{
	for (int id : views::iota(0, static_cast<int>(AudioGroupID::None)))
		m_audioGroups[static_cast<AudioGroupID>(id)] = make_unique<AudioGroup>();
}

bool SDLAudioManager::LoadWav(const string& filename, AudioGroupID groupID)
{
	AudioBuffer buffer;
	ReturnIfFalse(buffer.loadFromFile(filename, groupID));

	m_audioBuffers.insert({ filename, move(buffer) });
	return true;
}

bool SDLAudioManager::Unload(const string& filename) noexcept
{
	auto it = m_audioBuffers.find(filename);
	if (it == m_audioBuffers.end()) return false;

	m_audioBuffers.erase(it);
	return true;
}

void SDLAudioManager::SetVolume(AudioGroupID groupID, float volume) noexcept
{
	auto& audioGroup = m_audioGroups[groupID];
	audioGroup->volume = volume;

	for (auto& buffer : m_audioBuffers | views::values)
	{
		if (!buffer.IsPlaying()) continue;
		if (buffer.GetGroupID() != groupID) continue;

		buffer.SetVolume(volume);
	}
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
	auto it = m_audioBuffers.find(filename);
	if (it == m_audioBuffers.end()) return false;

	auto& buffer = it->second;
	ReturnIfFalse(buffer.SetVolume(GetVolume(buffer.GetGroupID())));
	buffer.Play();
	return true;
}

PlayState SDLAudioManager::GetPlayState(const string& filename)
{
	auto it = m_audioBuffers.find(filename);
	if (it == m_audioBuffers.end()) return PlayState::NotLoaded;

	return it->second.IsPlaying() ? PlayState::Playing : PlayState::Stopped;
}

void SDLAudioManager::Update() noexcept
{
	for (auto& buffer : m_audioBuffers | views::values)
	{
		if (!buffer.IsPlaying()) continue;
		buffer.Update();
	}
}

unique_ptr<IAudioManager> CreateAudioManager()
{
	return make_unique<SDLAudioManager>();
}