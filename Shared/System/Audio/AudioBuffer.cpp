#include "pch.h"
#include "AudioBuffer.h"
#include "../Public/IAudioManager.h"

AudioBuffer::AudioBuffer() :
	m_data{ nullptr, [](Uint8* p) { SDL_free(p); } },
	m_groupID{ AudioGroupID::None }
{}

bool AudioBuffer::loadFromFile(const string& filename, AudioGroupID groupID)
{
	Uint8* rawData = nullptr;
	ReturnIfFalse(SDL_LoadWAV(filename.c_str(), &m_spec, &rawData, &m_length));
	m_data.reset(rawData);

	m_stream = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_spec, nullptr, nullptr);
	if (!m_stream)
	{
		m_data.reset();
		return false;
	}

	m_groupID = groupID;
	return true;
}

bool AudioBuffer::SetVolume(float volume)
{
	return SDL_SetAudioStreamGain(m_stream, volume);
}

void AudioBuffer::Play()
{
	PushChunk();

	if (!m_resumed) {
		SDL_ResumeAudioStreamDevice(m_stream);
		m_resumed = true;
	}
}

bool AudioBuffer::IsPlaying() const noexcept
{
	return m_resumed;
}

void AudioBuffer::Update()
{
	if (m_offset >= m_length)
	{
		if (SDL_GetAudioStreamAvailable(m_stream) == 0) ResetPlay();
		return;
	}

	PushChunk();
}

void AudioBuffer::ResetPlay() noexcept
{
	m_offset = 0;
	m_resumed = false;
}

void AudioBuffer::PushChunk()
{
	Uint32 chunk = std::min(512u, m_length - m_offset);
	SDL_PutAudioStreamData(m_stream, m_data.get() + m_offset, chunk);
	m_offset += chunk;
}

AudioGroupID AudioBuffer::GetGroupID() const noexcept { return m_groupID; }