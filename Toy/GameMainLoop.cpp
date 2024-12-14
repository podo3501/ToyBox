#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIType.h"
#include "Utility.h"
#include "Config.h"
#include "UserInterface/UILayout.h"
#include "UserInterface/OButton.h"
#include "UserInterface/BGImage.h"
#include "UserInterface/TextArea.h"
#include "UserInterface/Panel.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

#ifdef USING_D3D12_AGILITY_SDK
extern "C"
{
    // Used to enable the "Agility SDK" components
    __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_SDK_VERSION;
    __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\D3D12\\";
}
#endif

GameMainLoop::~GameMainLoop() = default;
GameMainLoop::GameMainLoop(Window* window, IRenderer* renderer) :
    ::MainLoop(window, renderer),
    m_window{ window },
    m_renderer{ renderer }
{}

bool GameMainLoop::InitializeDerived()
{
    m_gamePanel = make_unique<Panel>("Main", GetRectResolution());
    m_renderer->AddComponent(m_gamePanel.get(), true);

    return true;
}

bool GameMainLoop::LoadResources()
{
    vector<ImageSource> normal
    {
        { L"UI/Blue/bar_square_large_l.png" },
        { L"UI/Blue/bar_square_large_m.png" },
        { L"UI/Blue/bar_square_large_r.png" },
    };
    vector<ImageSource> hover
    {
        { L"UI/Red/bar_square_large_l.png" },
        { L"UI/Red/bar_square_large_m.png" },
        { L"UI/Red/bar_square_large_r.png" },
    };
    vector<ImageSource> pressed
    {
        { L"UI/Gray/bar_square_large_l.png" },
        { L"UI/Gray/bar_square_large_m.png" },
        { L"UI/Gray/bar_square_large_r.png" },
    };

    UILayout layout({ 0, 0, 180, 48 }, Origin::Center);
    unique_ptr<OButton> button = make_unique<OButton>();
    button->SetImage("Button", layout, normal, hover, pressed);
    unique_ptr<OButton> button_2 = make_unique<OButton>();
    button_2->SetImage("Button2", layout, normal, hover, pressed);

    m_gamePanel->AddComponent(move(button), {0.5f, 0.5f});
    m_gamePanel->AddComponent(move(button_2), { 0.5f, 0.4f });

    vector<ImageSource> normal2{ { L"UI/Blue/check_square_color_cross.png" } };
    vector<ImageSource> hover2{ { L"UI/Blue/check_square_grey_cross.png" } };
    vector<ImageSource> pressed2{ { L"UI/Gray/check_square_grey_cross.png" } };

    layout.Set({ 0, 0, 32, 32 }, Origin::Center);
    unique_ptr<OButton> closeButton = make_unique<OButton>();
    closeButton->SetImage("CloseButton", layout, normal2, hover2, pressed2);
    m_gamePanel->AddComponent(move(closeButton), { 0.2f, 0.2f });

    layout.Set({ 0, 0, 250, 120 }, Origin::Center);
    map<wstring, wstring> fontFileList;
    fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
    fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
    unique_ptr<TextArea> textArea = make_unique<TextArea>();
    wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
    textArea->SetFont("TextArea", text, layout, fontFileList);

    m_gamePanel->AddComponent(move(textArea), { 0.2f, 0.7f });

    layout.Set({ 0, 0, 220, 190 }, Origin::LeftTop);
    ImageSource bgImageSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };
    unique_ptr<BGImage> bgImage = make_unique<BGImage>();
    bgImage->SetImage("BGImage", layout, bgImageSource);

    unique_ptr<Panel> panel = make_unique<Panel>("Panel", GetRectResolution());
    panel->AddComponent(move(bgImage), { 0.5f, 0.5f });
    Rectangle test = panel->GetArea();

    m_gamePanel->AddComponent(move(panel), { 0.f, 0.f });

    return true;
}

void GameMainLoop::Update(const DX::StepTimer* timer, InputManager* inputManager)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gamePanel->Update({}, inputManager);

    PIXEndEvent();
}