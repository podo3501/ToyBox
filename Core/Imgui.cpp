#include "pch.h"
#include "Imgui.h"
#include "../Toy/Window.h"
#include "../Core/Renderer.h"
#include "../Toy/Utils/CommonUtil.h"

constexpr int NUM_FRAMES_IN_FLIGHT = 2;

Imgui::Imgui(HWND hwnd) :
    m_hwnd{ hwnd },
    m_io{ nullptr }
{}

Imgui::~Imgui()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool Imgui::Initialize(ID3D12Device* device, DescriptorHeap* descHeap, DXGI_FORMAT format, size_t srvOffset)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    //m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ReturnIfFalse(ImGui_ImplWin32_Init(m_hwnd));
    ReturnIfFalse(ImGui_ImplDX12_Init(device, NUM_FRAMES_IN_FLIGHT, format, descHeap->Heap(),
        descHeap->GetCpuHandle(srvOffset),
        descHeap->GetGpuHandle(srvOffset)));

    //폰트 설정(제일 위에 있는 폰트가 index 0를 가지며 default 폰트이다.
    string ttfFilename = "D:\\ProgrammingStudy\\ToyBox\\Imgui\\misc\\fonts\\DroidSans.ttf";
    auto font = m_io->Fonts;
    font->AddFontFromFileTTF(ttfFilename.c_str(), 15.0f);
    font->AddFontFromFileTTF(ttfFilename.c_str(), 18.0f);
    
    return true;
}

void Imgui::AddComponent(IImguiComponent* comp)
{
    m_components.emplace_back(comp);
}

void Imgui::RemoveComponent(IImguiComponent* comp) noexcept
{
    erase(m_components, comp);
}

void Imgui::Render(ID3D12GraphicsCommandList* commandList)
{
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void Imgui::PrepareRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //여기서 ui 만드는 부분 호출한다.
    ranges::for_each(m_components, [this](const auto& item) 
        { 
            item->Render(m_io); 
        }
    );

    // Rendering
    ImGui::Render();

    // Update and Render additional Platform Windows
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Imgui::Reset()
{
    //imgui 리셋에 대해서 처리해야할 것이 있을 수 있다.
}

