#include "pch.h"
#include "Popup.h"
#include <shobjidl.h>

using namespace Tool;

Popup::Popup() = default;
Popup::~Popup() = default;

bool Popup::OpenFileDialog(wstring& filename)
{
    // COM 초기화
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return false;

    wstring initialPath = std::filesystem::current_path().wstring();
    IFileOpenDialog* pFileOpen = nullptr;

    // 파일 열기 대화상자 생성
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (SUCCEEDED(hr)) 
    {
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
                    filename = filePath;
                    CoTaskMemFree(filePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }

    // COM 종료
    CoUninitialize();

    return true;
}

void Popup::ShowErrorDialog(const string& errorMsg) noexcept
{
    m_errorDialog = true;
    m_errorMsg = errorMsg;
}

void Popup::Render() noexcept
{
    if (m_errorDialog) 
        ImGui::OpenPopup("Error");

    // 다이얼로그 정의
    if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(m_errorMsg.c_str());
        ImGui::Separator();
        ImVec2 buttonSize(100, 0);
        float windowWidth = ImGui::GetWindowSize().x;
        float centeredX = (windowWidth - buttonSize.x) / 2.0f;
        ImGui::SetCursorPosX(centeredX);
        if (ImGui::Button("OK", buttonSize))
        {
            m_errorDialog = false; // 다이얼로그 닫기
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}