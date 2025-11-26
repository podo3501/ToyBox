#pragma once
#include "../Public/IAudioManager.h"

struct AudioGroup;
class AudioBuffer;
class SDLAudioManager : public IAudioManager
{
public:
	~SDLAudioManager();
	SDLAudioManager();
	virtual bool Initialize() override;
	virtual bool LoadWav(const string& filename, AudioGroupID groupID) override;
	virtual bool Unload(const string& filename) noexcept override;
	virtual void SetVolume(AudioGroupID groupID, float volume) noexcept override;
	virtual bool Play(const string& filename) override;
	virtual PlayState GetPlayState(const string& filename) override;
	virtual void Update() noexcept override;

private:
	void CreateAudioGroup() noexcept;
	float GetVolume(AudioGroupID groupID) const noexcept;

	unordered_map<string, AudioBuffer> m_audioBuffers;
	unordered_map<AudioGroupID, unique_ptr<AudioGroup>> m_audioGroups;
};