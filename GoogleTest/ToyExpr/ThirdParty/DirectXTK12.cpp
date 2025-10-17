#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Shared/System/Public/IAudioManager.h"
#include "Shared/System/Public/IInputManager.h"
#include "Shared/Window/Window.h"

namespace ThirdParty
{
    TEST(DirectXTK12, Mouse)
    {
        RECT windowRect = { 0, 0, 800, 600 };
        auto window = CreateWindowInstance(GetModuleHandle(nullptr), SW_HIDE, windowRect);
        unique_ptr<IInputManager> manager = CreateInputManager(window->GetHandle());

        Locator<IInputManager>::Provide(manager.get());
        IInputManager* inputManager = Locator<IInputManager>::GetService();

        inputManager->SetMouseStartOffset({ 10, 10 });
        inputManager->Update();
        MouseState mouseState = inputManager->GetMouseState();
    }

    TEST(DirectXTK12, Sound)
    {
        unique_ptr<IAudioManager> audioManager = CreateAudioManager(L"../Resources/");
        Locator<IAudioManager>::Provide(audioManager.get());

        IAudioManager* currAudioManager = Locator<IAudioManager>::GetService();
        audioManager->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);
        audioManager->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);

        audioManager->SetVolume("click1", 0.0f); // 테스트 이기 때문에 소리 안나게 설정.
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