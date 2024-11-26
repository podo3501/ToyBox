#include "pch.h"
#include "Popup.h"
#include <shobjidl.h>
#include <wrl/wrappers/corewrappers.h>

using namespace Tool;

Popup::Popup() = default;
Popup::~Popup() = default;

bool Popup::OpenFileDialog(wstring& filename)
{
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);

    wstring initialPath = L"../Resources/";
    //wstring initialPath = std::filesystem::current_path().wstring();
    IFileOpenDialog* pFileOpen = nullptr;

    // ���� ���� ��ȭ���� ����
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (!SUCCEEDED(hr))
        return false;
    
    // �ʱ� ���� ����
    if (!initialPath.empty()) {
        IShellItem* pInitialFolder = nullptr;
        hr = SHCreateItemFromParsingName(initialPath.c_str(), nullptr, IID_PPV_ARGS(&pInitialFolder));
        if (SUCCEEDED(hr)) {
            hr = pFileOpen->SetFolder(pInitialFolder); // �ʱ� ���� ����
            pInitialFolder->Release();
        }
    }

    // ��ȭ���� ǥ��
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

    // ���̾�α� ����
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
            m_errorDialog = false; // ���̾�α� �ݱ�
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}