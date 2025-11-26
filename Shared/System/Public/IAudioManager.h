#pragma once
#include <string>
#include <memory>

enum class AudioGroupID : int
{
	Master,
	BGM,
	SFX,
	UI,
	System,
	None,
};

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
	virtual bool LoadWav(const string& filename, AudioGroupID groupID) = 0;
	virtual bool Unload(const string& filename) noexcept = 0;
	virtual void SetVolume(AudioGroupID groupID, float volume) noexcept = 0;
	virtual bool Play(const string& filename) = 0;
	virtual PlayState GetPlayState(const string& filename) = 0;
	virtual void Update() noexcept = 0;
};

unique_ptr<IAudioManager> CreateAudioManager();