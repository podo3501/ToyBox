#include "pch.h"
#include "Toy/Locator/InputLocator.h"
#include "Toy/Locator/AudioLocator.h"
#include "Shared/Window/Window.h"

namespace ThirdParty
{
    TEST(DirectXTK12, Mouse)
    {
        RECT windowRect = { 0, 0, 800, 600 };
        auto window = CreateWindowInstance(GetModuleHandle(nullptr), SW_HIDE, windowRect);
        unique_ptr<IInputManager> inputManager = CreateInputManager(window->GetHandle());
        InputLocator::Provide(inputManager.get());

        auto input = InputLocator::GetService();
        input->SetMouseStartOffset({ 10, 10 });
        input->Update();
        MouseState mouseState = input->GetMouseState();
    }

    TEST(DirectXTK12, Sound)
    {
        unique_ptr<IAudioManager> audioManager = CreateAudioManager(L"../Resources/");
        AudioLocator::Provide(audioManager.get());

        auto audio = AudioLocator::GetService();
        audio->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);
        audio->LoadWavFile("click1", L"UI/Sound/click1.wav", 4);

        audio->SetVolume("click1", 0.0f); // 테스트 이기 때문에 소리 안나게 설정.
        audio->Play("click1");
        while (audio->IsPlaying("click1"))
        {
            audio->Update();
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        audio->Unload("click1");
        audio->Unload("click1");
    }
}