#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Shared/System/Public/IAudioManager.h"

namespace ThirdParty
{
    TEST(DirectXTK12, Sound)
    {
        unique_ptr<IAudioManager> audioManager = CreateAudioManager(L"../Resources/");
        Locator<IAudioManager>::Provide(audioManager.get());

        IAudioManager* currAudioManager = Locator<IAudioManager>::GetService();
        audioManager->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);
        audioManager->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);

        audioManager->SetVolume("click1", 0.0f); // �׽�Ʈ �̱� ������ �Ҹ� �ȳ��� ����.
        audioManager->Play("click1");
        while (audioManager->IsPlaying("click1"))
        {
            audioManager->Update();
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        audioManager->Unload("click1");
        audioManager->Unload("click1");
    }
}