#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIType.h"
#include "Utility.h"
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
    m_button = make_unique<Button>();
    m_button2 = make_unique<Button>();
    m_closeButton = make_unique<Button>();
    m_textArea = make_unique<TextArea>();
    m_panel = make_unique<Panel>();

    return true;
}

bool GameMainLoop::LoadResources(const wstring& resPath)
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

    UILayout layout({ 0, 0, 180, 48 }, { 0.5f, 0.5f }, Origin::Center);
    m_button->SetImage(resPath, m_renderer, normal, hover, pressed, layout);
    layout.Set({ 0.5f, 0.4f });
    m_button2->SetImage(resPath, m_renderer, normal, hover, pressed, layout);

    m_renderer->AddRenderItem(m_button.get());
    m_renderer->AddRenderItem(m_button2.get());

    vector<ImageSource> normal2{ { L"UI/Blue/check_square_color_cross.png" } };
    vector<ImageSource> hover2{ { L"UI/Blue/check_square_grey_cross.png" } };
    vector<ImageSource> pressed2{ { L"UI/Gray/check_square_grey_cross.png" } };

    layout.Set({ 0, 0, 32, 32 }, { 0.2f, 0.2f }, Origin::Center);
    m_closeButton->SetImage(resPath, m_renderer, normal2, hover2, pressed2, move(layout));
    m_renderer->AddRenderItem(m_closeButton.get());

    layout.Set({ 0, 0, 250, 120 }, { 0.2f, 0.7f }, Origin::Center);
    map<wstring, wstring> fontFileList;
    fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
    fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
    m_textArea->SetFont(resPath, m_renderer, fontFileList, layout);

    m_renderer->AddRenderItem(m_textArea.get());

    layout.Set({ 0, 0, 220, 190 }, { 0.0f, 0.0f }, Origin::LeftTop);
    ImageSource bgImageSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };
    unique_ptr<BGImage> bgImage = make_unique<BGImage>();
    bgImage->SetImage(resPath, m_renderer, bgImageSource, layout);
    //m_renderer->AddRenderItem(m_dialog.get());
    m_panel->AddRenderItem({ 0.5f, 0.5f }, move(bgImage));
    Rectangle test = m_panel->GetArea();

    m_renderer->AddRenderItem(m_panel.get());
    

    return true;
}

bool GameMainLoop::SetDatas(IGetValue* getValue)
{
    auto result = m_textArea->SetText(getValue,
        L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>");

    return result;
}

void GameMainLoop::Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_button->Update(resolution, mouseTracker);
    m_button2->Update(resolution, mouseTracker);
    m_closeButton->Update(resolution, mouseTracker);
    m_textArea->Update(resolution);
    m_panel->Update(resolution);

    PIXEndEvent();
}