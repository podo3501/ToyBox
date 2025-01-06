#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIType.h"
#include "Utility.h"
#include "Config.h"
#include "UserInterface/TextArea.h"
#include "UserInterface/Panel.h"
#include "UserInterface/ImageGrid1.h"
#include "UserInterface/ImageGrid3.h"
#include "UserInterface/ImageGrid9.h"
#include "UserInterface/UIUtility.h"
#include "UserInterface/Button.h"

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
    m_gamePanel = make_unique<Panel>("Main", RectangleToXMUINT2(GetRectResolution()));
    m_renderer->AddRenderComponent(m_gamePanel.get());

    return true;
}

bool GameMainLoop::LoadResources()
{
    UILayout loButton({ 180, 48 }, Origin::Center);
    UILayout loImgGrid({ 180, 48 }, Origin::LeftTop);
    ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 67, 35, 22, 48}, { 89, 35, 4, 48 }, { 93, 35, 22, 48 } } };
    ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 118, 35, 22, 48}, { 140, 35, 4, 48 }, { 144, 35, 22, 48 } } };
    ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 169, 35, 22, 48}, { 191, 35, 4, 48 }, { 195, 35, 22, 48 } } };

    std::unique_ptr<Button> button = std::make_unique<Button>();
    button->SetImage("Button", loButton,
        CreateImageGrid<ImageGrid3>("Button_normal", loImgGrid, normal),
        CreateImageGrid<ImageGrid3>("Button_hover", loImgGrid, hover),
        CreateImageGrid<ImageGrid3>("Button_pressed", loImgGrid, pressed));
    std::unique_ptr<Button> button2 = std::make_unique<Button>();
    button2->SetImage("Button", loButton,
        CreateImageGrid<ImageGrid3>("Button_normal", loImgGrid, normal),
        CreateImageGrid<ImageGrid3>("Button_hover", loImgGrid, hover),
        CreateImageGrid<ImageGrid3>("Button_pressed", loImgGrid, pressed));

    m_gamePanel->AddComponent(move(button), { 400, 300 });
    m_gamePanel->AddComponent(move(button2), { 400, 240 });

    UILayout loButton2({ 32, 32 }, Origin::Center);
    UILayout loImgGrid2({ 32, 32 }, Origin::LeftTop);
    ImageSource normal2{ { L"UI/Texture/Test_01.png" }, { { 0, 0, 32, 32} } };
    ImageSource hover2{ { L"UI/Texture/Test_01.png" }, { { 35, 0, 32, 32} } };
    ImageSource pressed2{ { L"UI/Texture/Test_01.png" }, { { 70, 0, 32, 32} } };

    std::unique_ptr<Button> closeButton = std::make_unique<Button>();
    closeButton->SetImage("Button", loButton2,
        CreateImageGrid<ImageGrid1>("Button_normal", loImgGrid2, normal2),
        CreateImageGrid<ImageGrid1>("Button_hover", loImgGrid2, hover2),
        CreateImageGrid<ImageGrid1>("Button_pressed", loImgGrid2, pressed2));

    m_gamePanel->AddComponent(move(closeButton), { 160, 120 });

    UILayout layout({ 250, 120 }, Origin::Center);
    map<wstring, wstring> fontFileList;
    fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
    fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
    unique_ptr<TextArea> textArea = make_unique<TextArea>();
    wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
    textArea->SetFont("TextArea", text, layout, fontFileList);

    m_gamePanel->AddComponent(move(textArea), { 160, 420 });

    layout.Set({ 210, 150 }, Origin::LeftTop);
    ImageSource img9Source{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };

    unique_ptr<ImageGrid9> imgGrid9 = make_unique<ImageGrid9>();
    imgGrid9->SetImage("ImgGrid9", layout, img9Source);
    m_gamePanel->AddComponent(move(imgGrid9), { 400, 300 });

    m_renderer->LoadComponent(m_gamePanel.get());

    return true;
}

void GameMainLoop::Update(const DX::StepTimer* timer, InputManager* inputManager)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gamePanel->ProcessUpdate({}, inputManager);

    PIXEndEvent();
}