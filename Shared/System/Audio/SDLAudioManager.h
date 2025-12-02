#pragma once
#include "../Public/IAudioManager.h"

struct ISoundTableReader;
struct AudioGroup;
class EffectSound;
class NormalSound;
class SDLAudioManager : public IAudioManager
{
public:
	~SDLAudioManager();
	SDLAudioManager() = delete;
	SDLAudioManager(unique_ptr<ISoundTableReader> soundReader);
	virtual bool Initialize() override;
	virtual bool LoadSound(const string& filename, AudioGroupID groupID) override;
	bool LoadSound(const string& index);
	virtual bool Unload(const string& filename) noexcept override;
	virtual void SetVolume(AudioGroupID groupID, float volume) noexcept override;
	virtual bool Play(const string& filename) override;
	virtual PlayState GetPlayState(const string& filename) override;
	virtual void Update() noexcept override;

private:
	void CreateAudioGroup() noexcept;
	float GetVolume(AudioGroupID groupID) const noexcept;

	unique_ptr<ISoundTableReader> m_reader;
	unique_ptr<EffectSound> m_effectSound;
	unique_ptr<NormalSound> m_normalSound;
	unordered_map<AudioGroupID, unique_ptr<AudioGroup>> m_audioGroups;
};