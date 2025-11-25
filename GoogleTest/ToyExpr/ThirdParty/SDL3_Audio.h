#pragma once
#include "SDL3/SDL.h"

enum class PlayState : int
{
	NotLoaded,
	Stopped,
	Playing
};

struct IAudioManager
{
	virtual ~IAudioManager() = default;
	virtual bool Initialize() = 0;
	virtual bool LoadWav(const string& filename) = 0;
	virtual bool Unload(const string& filename) noexcept = 0;
	virtual bool Play(const string& filename) = 0;
	virtual PlayState GetPlayState(const string& filename) = 0;
	virtual void Update() noexcept = 0;
};

struct AudioBuffer
{
	SDL_AudioSpec spec{};
	unique_ptr<Uint8, function<void(Uint8*)>> data{ nullptr, [](Uint8* p) { SDL_free(p); } };
	Uint32 length{ 0 };
	Uint32 offset{ 0 };
	bool resumed{ false };
	SDL_AudioStream* stream{ nullptr };

	void ResetPlay() noexcept
	{
		offset = 0;
		resumed = false;
	}

	bool loadFromFile(const string& filename)
	{
		Uint8* rawData = nullptr;
		ReturnIfFalse(SDL_LoadWAV(filename.c_str(), &spec, &rawData, &length));
		data.reset(rawData);

		stream = SDL_OpenAudioDeviceStream(
			SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
		if (!stream)
		{
			data.reset();
			return false;
		}
		
		return true;
	}

	void Play()
	{
		Uint32 chunk = std::min(512u, length - offset);
		SDL_PutAudioStreamData(stream, data.get() + offset, chunk);
		offset += chunk;

		if (!resumed) {
			SDL_ResumeAudioStreamDevice(stream);
			resumed = true;
		}
	}

	bool IsPlaying() const noexcept
	{
		return resumed;
	}

	void Update()
	{
		if (offset >= length)
		{
			if (SDL_GetAudioStreamAvailable(stream) == 0) ResetPlay();
			return;
		}

		Uint32 chunk = std::min(512u, length - offset);
		SDL_PutAudioStreamData(stream, data.get() + offset, chunk);
		offset += chunk;
	}
};

class SDLAudioManager : public IAudioManager
{
public:
	~SDLAudioManager();
	SDLAudioManager();
	virtual bool Initialize() override;
	virtual bool LoadWav(const string& filename) override;
	virtual bool Unload(const string& filename) noexcept override;
	virtual bool Play(const string& filename) override;
	virtual PlayState GetPlayState(const string& filename) override;
	virtual void Update() noexcept override;

private:
	unordered_map<string, AudioBuffer> m_audioBuffers;
};

SDLAudioManager::~SDLAudioManager() { SDL_QuitSubSystem(SDL_INIT_AUDIO); }
SDLAudioManager::SDLAudioManager() = default;
bool SDLAudioManager::Initialize()
{
	bool isInit = SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO;
	if (isInit) return true;

	return SDL_InitSubSystem(SDL_INIT_AUDIO);
}

bool SDLAudioManager::LoadWav(const string& filename)
{
	AudioBuffer buffer;
	ReturnIfFalse(buffer.loadFromFile(filename));

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

bool SDLAudioManager::Play(const string& filename)
{
	auto it = m_audioBuffers.find(filename);
	if (it == m_audioBuffers.end()) return false;

	it->second.Play();
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

class SDL3_Audio : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;
	string GetWavFilename() const noexcept { return "../Resources/Sound/Wav/click5.wav"; }

	unique_ptr<IAudioManager> m_audioManager;
};

void SDL3_Audio::SetUp()
{
	m_audioManager = CreateAudioManager();
	m_audioManager->Initialize();
}

void SDL3_Audio::TearDown()
{
	SDL_Quit();
}

