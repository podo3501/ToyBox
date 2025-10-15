#pragma once
#include <string>
#include <memory>

struct IAudioManager
{
    virtual ~IAudioManager() = default;
    virtual bool Initialize() = 0;
    virtual bool LoadWavFile(const std::string& name, const std::wstring& filePath, int poolSize) = 0;
    virtual void Unload(const std::string& key) noexcept = 0;
    virtual void Play(const std::string& key) = 0;
    virtual bool SetVolume(const std::string& key, float volume) noexcept = 0;
    virtual bool Update() = 0;
    virtual bool IsPlaying(const std::string& key) const noexcept = 0;
};

std::unique_ptr<IAudioManager> CreateAudioManager(const std::wstring& resPath);