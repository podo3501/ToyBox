#include "pch.h"
#include "Imgui.h"
#include "../Toy/Window.h"
#include "../Core/Renderer.h"
#include "../Toy/Utility.h"

constexpr int NUM_FRAMES_IN_FLIGHT = 2;

Imgui::Imgui(HWND hwnd) :
    m_hwnd{ hwnd },
    m_io{ nullptr },
    m_descriptorHeap{ nullptr }
{}

Imgui::~Imgui()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool Imgui::Initialize(ID3D12Device* device)
{
    m_descriptorHeap = make_unique<DescriptorHeap>(device, 100);
    ID3D12DescriptorHeap* descriptorHeap = m_descriptorHeap->Heap();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ReturnIfFalse(ImGui_ImplWin32_Init(m_hwnd));

    ReturnIfFalse(ImGui_ImplDX12_Init(device, NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_B8G8R8A8_UNORM, descriptorHeap,
        descriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        descriptorHeap->GetGPUDescriptorHandleForHeapStart()));

    return true;
}

void Imgui::AddItem(IImguiItem* item)
{
    m_items.emplace_back(item);
}

void Imgui::Render(ID3D12GraphicsCommandList* commandList)
{
    ID3D12DescriptorHeap* heaps[] = { m_descriptorHeap->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(size(heaps)), heaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void Imgui::PrepareRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //여기서 ui 만드는 부분 호출한다.
    ranges::for_each(m_items, [this](const auto& item) { item->Render(m_io); });

    // Rendering
    ImGui::Render();
}

void Imgui::Reset()
{
    m_descriptorHeap.reset();
    //imgui 리셋에 대해서 처리해야할 것이 있을 수 있다.
}


