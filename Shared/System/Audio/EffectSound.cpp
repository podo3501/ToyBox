#include "pch.h"
#include "EffectSound.h"
#include "EffectSoundBuffer.h"
#include "../Public/IAudioManager.h"
#include "SDL3/SDL_init.h"

EffectSound::~EffectSound() = default;
EffectSound::EffectSound() = default;
bool EffectSound::Initialize()
{
	bool isInit = SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO;
	if (isInit) return true;

	return SDL_InitSubSystem(SDL_INIT_AUDIO);
}

bool EffectSound::LoadWav(const string& filename, AudioGroupID groupID, float volume)
{
	EffectSoundBuffer buffer;
	ReturnIfFalse(buffer.LoadFromFile(filename, groupID, volume));

	m_effectSoundBuffers.insert({ filename, move(buffer) });
	return true;
}

bool EffectSound::Unload(const string& filename) noexcept
{
	auto it = m_effectSoundBuffers.find(filename);
	if (it == m_effectSoundBuffers.end()) return false;

	m_effectSoundBuffers.erase(it);
	return true;
}

void EffectSound::SetVolume(AudioGroupID groupID, float volume) noexcept
{
	for (auto& buffer : m_effectSoundBuffers | views::values)
	{
		if (!buffer.IsPlaying()) continue;
		if (buffer.GetGroupID() != groupID) continue;

		buffer.SetVolume(volume);
	}
}

bool EffectSound::Play(const string& filename)
{
	auto it = m_effectSoundBuffers.find(filename);
	if (it == m_effectSoundBuffers.end()) return false;

	it->second.Play();
	return true;
}

PlayState EffectSound::GetPlayState(const string& filename) const noexcept
{
	auto it = m_effectSoundBuffers.find(filename);
	if (it == m_effectSoundBuffers.end()) return PlayState::NotLoaded;

	return it->second.IsPlaying() ? PlayState::Playing : PlayState::Stopped;
}

void EffectSound::Update() noexcept
{
	for (auto& buffer : m_effectSoundBuffers | views::values)
	{
		if (!buffer.IsPlaying()) continue;
		buffer.Update();
	}
}