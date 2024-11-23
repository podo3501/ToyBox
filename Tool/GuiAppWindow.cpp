#include "pch.h"
#include "GuiAppWindow.h"
#include "GuiWidget.h"
#include "../Include/IRenderScene.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"

GuiAppWindow::~GuiAppWindow() = default;
GuiAppWindow::GuiAppWindow(IRenderer* renderer) :
    m_renderer{ renderer },
    m_component{ nullptr }
{
    m_renderer->AddImguiComponent(this);
}

bool GuiAppWindow::Create(unique_ptr<IRenderScene> scene, const XMUINT2& size)
{
    //ReturnIfFalse(m_renderer->CreateRenderTexture(size, scene.get(), m_textureID));
    //auto dialog = scene->GetComponent("Dialog_clone");
    //m_component = dialog->GetComponent("BGImage_clone");
    //m_size = size;

    ////임시로 bgImage가 선택되었다고 가정한다.
    //m_guiWidget = make_unique<GuiWidget>(m_renderer);
    //unique_ptr<UIComponent> clone = m_component->Clone();
    //
    //ReturnIfFalse(m_guiWidget->Create(move(clone)));
    
    m_scene = move(scene);
    return true;
}

void GuiAppWindow::Update()
{
    //m_guiWidget->Update();
    m_scene->Update(nullptr);
}

#include <shobjidl.h> // IFileOpenDialog 정의
void ShowFileOpenDialog(const std::wstring& initialPath, std::wstring& selectedFilePath) {
    // COM 초기화
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return;

    IFileOpenDialog* pFileOpen = nullptr;

    // 파일 열기 대화상자 생성
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (SUCCEEDED(hr)) {
        // 초기 폴더 설정
        if (!initialPath.empty()) {
            IShellItem* pInitialFolder = nullptr;
            hr = SHCreateItemFromParsingName(initialPath.c_str(), nullptr, IID_PPV_ARGS(&pInitialFolder));
            if (SUCCEEDED(hr)) {
                pFileOpen->SetFolder(pInitialFolder); // 초기 폴더 설정
                pInitialFolder->Release();
            }
        }

        // 대화상자 표시
        hr = pFileOpen->Show(nullptr);
        if (SUCCEEDED(hr)) {
            IShellItem* pItem = nullptr;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR filePath = nullptr;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
                if (SUCCEEDED(hr)) {
                    selectedFilePath = filePath;
                    CoTaskMemFree(filePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }

    // COM 종료
    CoUninitialize();
}

void GuiAppWindow::Render(ImGuiIO* io)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                wstring path = std::filesystem::current_path().wstring();
                wstring selectedFilename{};
                ShowFileOpenDialog(path, selectedFilename);
                
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("App Window", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    static bool bPicking{ false };
    ImVec2 curWindowPos = ImGui::GetWindowPos();
    ImVec2 startPos{ curWindowPos.x + 0.f, curWindowPos.y + 0.f + ImGui::GetFrameHeight() };
    if (/*io->MouseDown[0] && */io->MouseReleased[0])
    {
        Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

        Vector2 realMousePos{};
        realMousePos.x = mousePos.x - startPos.x;
        realMousePos.y = mousePos.y - startPos.y;

        //클릭했을때 픽킹이 되었는지 확인
        //if (m_component->IsPicking(realMousePos))
        //    m_guiWidget->SetVisible(true);
    }

    ImGui::Image(m_textureID, { static_cast<float>(m_size.x), static_cast<float>(m_size.y) });
    //ImGui::Image(m_textureID, { static_cast<float>(800), static_cast<float>(600) });
    ImGui::End();
    return;
}