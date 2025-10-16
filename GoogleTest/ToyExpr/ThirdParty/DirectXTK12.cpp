#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Shared/System/Public/IAudioManager.h"
#include "Shared/System/Input.h"
#include "Shared/Window/Window.h"
#include "DirectXTK12/Mouse.h"

struct MouseState
{
    int32_t x{ 0 };
    int32_t y{ 0 };
    bool leftButton{ false };
};

struct IMouse
{
    ~IMouse() = default;
    virtual MouseState GetState() const noexcept = 0;
};
unique_ptr<IMouse> CreateMouse();

class Mouse : public IMouse
{
public:
    ~Mouse() = default;
    Mouse() = default;
    virtual MouseState GetState() const noexcept override 
    { 
        auto dxState = Input::GetMouseState();
        return { dxState.x, dxState.y, dxState.leftButton };
    }
};

unique_ptr<IMouse> CreateMouse()
{
    return make_unique<Mouse>();
}

namespace ThirdParty
{
    TEST(DirectXTK12, Mouse)
    {
        unique_ptr<IMouse> mouse = CreateMouse();

        Locator<IMouse>::Provide(mouse.get());
        IMouse* currMouse = Locator<IMouse>::GetService();

        MouseState mouseState = currMouse->GetState();
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