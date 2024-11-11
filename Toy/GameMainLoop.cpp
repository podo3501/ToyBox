#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIType.h"
#include "Utility.h"
#include "UserInterface/Scene.h"
#include "UserInterface/UILayout.h"
#include "UserInterface/Button.h"
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
    m_gameScene = make_unique<Scene>(m_renderer);
    m_renderer->AddLoadScene(m_gameScene.get());
    m_renderer->AddRenderScene(m_gameScene.get());

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
    unique_ptr<Button> button = make_unique<Button>();
    button->SetImage("Button", { 0.5f, 0.5f }, layout, normal, hover, pressed);
    unique_ptr<Button> button_2 = make_unique<Button>();
    button_2->SetImage("Button2", { 0.5f, 0.4f }, layout, normal, hover, pressed);

    m_gameScene->AddRenderItem({0.f, 0.f}, move(button));
    m_gameScene->AddRenderItem({ 0.f, 0.f }, move(button_2));

    vector<ImageSource> normal2{ { L"UI/Blue/check_square_color_cross.png" } };
    vector<ImageSource> hover2{ { L"UI/Blue/check_square_grey_cross.png" } };
    vector<ImageSource> pressed2{ { L"UI/Gray/check_square_grey_cross.png" } };

    layout.Set({ 0, 0, 32, 32 }, Origin::Center);
    unique_ptr<Button> closeButton = make_unique<Button>();
    closeButton->SetImage("CloseButton", { 0.2f, 0.2f }, layout, normal2, hover2, pressed2);
    m_gameScene->AddRenderItem({ 0.f, 0.f }, move(closeButton));

    layout.Set({ 0, 0, 250, 120 }, Origin::Center);
    map<wstring, wstring> fontFileList;
    fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
    fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
    unique_ptr<TextArea> textArea = make_unique<TextArea>();
    textArea->SetFont("TextArea", { 0.2f, 0.7f }, layout, fontFileList);

    m_gameScene->AddRenderItem({ 0.f, 0.f }, move(textArea));

    layout.Set({ 0, 0, 220, 190 }, Origin::LeftTop);
    ImageSource bgImageSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };
    unique_ptr<BGImage> bgImage = make_unique<BGImage>();
    bgImage->SetImage("BGImage", { 0.f, 0.f }, layout, bgImageSource);

    unique_ptr<Panel> panel = make_unique<Panel>();
    panel->AddComponent(move(bgImage), { 0.5f, 0.5f });
    Rectangle test = panel->GetArea();

    m_gameScene->AddRenderItem({ 0.f, 0.f }, move(panel));

    return true;
}

bool GameMainLoop::SetDatas(IGetValue* getValue)
{
    auto renderItem = m_gameScene->GetRenderItem("TextArea");
    TextArea* textArea = static_cast<TextArea*>(renderItem);
    auto result = textArea->SetText(getValue,
        L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>");

    return result;
}

void GameMainLoop::Update(const DX::StepTimer* timer, const Mouse::ButtonStateTracker* mouseTracker)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gameScene->Update(mouseTracker);

    PIXEndEvent();
}