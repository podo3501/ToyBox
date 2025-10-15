#pragma once
#include "../Public/IAudioManager.h"
#include "Utils/Memory.h"

struct SoundData
{
    unique_ptr<SoundEffect> effect;
    vector<unique_ptr<SoundEffectInstance>> pool;
};

class AudioManager : public IAudioManager
{
public:
    ~AudioManager();
    AudioManager() = delete;
    AudioManager(const wstring& resPath);
    virtual bool Initialize() override;
    virtual bool LoadWavFile(const string& name, const wstring& filePath, int poolSize) override;
    virtual void Unload(const string& key) noexcept override;
    virtual void Play(const string& key);
    virtual bool SetVolume(const string& key, float volume) noexcept override;
    virtual bool Update() override;
    virtual bool IsPlaying(const string& key) const noexcept override;

private:
    wstring m_resPath;
    unique_ptr<AudioEngine> m_audioEngine;
    unordered_map<string, RefCounted<SoundData>> m_sounds;
};