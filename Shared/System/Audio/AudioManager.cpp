#include "pch.h"
#include "AudioManager.h"

AudioManager::~AudioManager()
{
    AssertMsg(m_sounds.empty(), "All sounds must be released");

    CoUninitialize();
}

AudioManager::AudioManager(const wstring& resPath) :
    m_audioEngine{ nullptr },
    m_resPath{ resPath }
{}

bool AudioManager::Initialize()
{
    auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (!SUCCEEDED(hr))
        return false;

    m_audioEngine = make_unique<AudioEngine>();

    return true;
}

bool AudioManager::LoadWavFile(const string& key, const wstring& filePath, int poolSize)
{
    auto find = m_sounds.find(key);
    if (find != m_sounds.end())
    {
        find->second.IncRef();
        return true;
    }

    RefCounted <SoundData> data;
    wstring fullFilePath = m_resPath + filePath;
    auto soundEffect = make_unique<SoundEffect>(m_audioEngine.get(), fullFilePath.c_str());
    for (int i = 0; i < poolSize; ++i)
        data->pool.push_back(soundEffect->CreateInstance());
    data->effect = move(soundEffect);

    m_sounds[key] = move(data);

    return true;
}

void AudioManager::Unload(const string& key) noexcept
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end())
        return;

    auto& soundData = it->second;
    if (soundData.DecRef()) //레퍼런스 카운터를 내리고 문제 없다면 리턴 false라면 실제로 지움.
        return;

    for (auto& instance : soundData->pool)
        if (instance)
            instance->Stop();

    soundData->pool.clear();
    soundData->effect.reset();
    m_sounds.erase(it);
}

void AudioManager::Play(const string& key)
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return;

    for (auto& inst : it->second->pool)
        if (inst->GetState() == SoundState::STOPPED)
            inst->Play();
}

bool AudioManager::SetVolume(const string& key, float volume) noexcept
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return false;

    for (auto& inst : it->second->pool)
        inst->SetVolume(volume);

    return true;
}

bool AudioManager::Update()
{
    return m_audioEngine->Update();
}

bool AudioManager::IsPlaying(const string& key) const noexcept
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return false;

    const auto& currPool = it->second->pool;
    return ranges::any_of(currPool, [](const auto& inst) {
        return inst->GetState() == SoundState::PLAYING; });
}

unique_ptr<IAudioManager> CreateAudioManager(const wstring& resPath)
{
    auto audioManager = make_unique<AudioManager>(resPath);
    if (!audioManager->Initialize())
        return nullptr;

    return audioManager;
}