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
}