#include "pch.h"
#include "SDL3/SDL.h"
#include "Shared/Framework/Locator.h"

struct IAudioManager
{
    virtual ~IAudioManager() = default;
    virtual bool Initialize() = 0;
    virtual bool LoadWavFile(const string& name, const wstring& filePath, int poolSize) = 0;
    virtual void Unload(const string& key) noexcept = 0;
    virtual void Play(const string& key) = 0;
    virtual bool SetVolume(const string& key, float volume) noexcept = 0;
    virtual bool Update() = 0;
    virtual bool IsPlaying(const string& key) const noexcept = 0;
};

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
    unordered_map<string, SoundData> m_sounds;
};

AudioManager::~AudioManager()
{
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
    if (m_sounds.find(key) != m_sounds.end()) 
        return false;

    SoundData data;
    wstring fullFilePath = m_resPath + filePath;
    data.effect = make_unique<SoundEffect>(m_audioEngine.get(), fullFilePath.c_str());

    for (int i = 0; i < poolSize; ++i)
        data.pool.push_back(data.effect->CreateInstance());

    m_sounds[key] = move(data);

    return true;
}

void AudioManager::Unload(const string& key) noexcept
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end())
        return;

    for (auto& instance : it->second.pool)
        if (instance)
            instance->Stop();

    it->second.pool.clear();
    it->second.effect.reset();
    m_sounds.erase(it);
}

void AudioManager::Play(const string& key)
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return;

    for (auto& inst : it->second.pool)
        if (inst->GetState() == SoundState::STOPPED)
            inst->Play();
}

bool AudioManager::SetVolume(const string& key, float volume) noexcept
{
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return false;

    for (auto& inst : it->second.pool)
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
    
    const auto& currPool = it->second.pool;
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

namespace ThirdParty
{
    TEST(DirectXTK12, Sound)
    {
        unique_ptr<IAudioManager> audioManager = CreateAudioManager(L"../Resources/");
        Locator<IAudioManager>::Provide(audioManager.get());

        IAudioManager* currAudioManager = Locator<IAudioManager>::GetService();
        audioManager->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);

        audioManager->SetVolume("click1", 0.0f); // 테스트 이기 때문에 소리 안나게 설정.
        audioManager->Play("click1");
        while (audioManager->IsPlaying("click1"))
        {
            audioManager->Update();
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }

	TEST(SDL3, Initialize)
	{
	}
}