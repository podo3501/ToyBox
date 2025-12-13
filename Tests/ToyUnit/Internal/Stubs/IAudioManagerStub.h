#pragma once
#include "Shared/System/Public/IAudioManager.h"

struct IAudioManagerStub : public IAudioManager
{
	virtual ~IAudioManagerStub() = default;
	virtual bool Initialize() { return true; }
	virtual bool LoadSound(const std::string& index) { index; return true; }
	virtual bool Unload(const std::string& index) noexcept { index; return true; }
	virtual void SetVolume(AudioGroupID groupID, float volume) noexcept {}
	virtual bool Play(const std::string& index) { index; return true; }
	virtual PlayState GetPlayState(const std::string& index) { return PlayState::NotLoaded; }
	virtual void Update() noexcept {}
};