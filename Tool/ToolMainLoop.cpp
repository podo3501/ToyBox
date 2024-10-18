#include "pch.h"
#include "ToolMainLoop.h"
#include "../Include/IRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/Window.h"
#include "../Core/Renderer.h"

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

static int const NUM_FRAMES_IN_FLIGHT = 2;

class ImguiRender : public IRenderItem
{
public:
    ImguiRender(Renderer* renderer) : m_renderer{ renderer } {}
    virtual bool LoadResources(ILoadData* load) override
    {
        return true;
    }

    virtual void Render(IRender* render) override
    {
        auto commandList = m_renderer->GetCommandList();
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
    }
    
private:
    Renderer* m_renderer{ nullptr };
};

unique_ptr<ImguiRender> imguiRender{ nullptr };

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ToolMainLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    return 0;
}

ToolMainLoop::~ToolMainLoop() = default;
ToolMainLoop::ToolMainLoop() :
    m_io{ nullptr }
{}

bool ToolMainLoop::InitializeDerived()
{
    m_window->AddWndProcListener([toolMainLoop = this](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return toolMainLoop->WndProc(wnd, msg, wp, lp); });

    Renderer* renderer = static_cast<Renderer*>(m_renderer.get());
    imguiRender = make_unique<ImguiRender>(renderer);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_window->GetHandle());
    ID3D12DescriptorHeap* heaps = renderer->GetDescriptorHeap()->Heap();

    ImGui_ImplDX12_Init(renderer->GetDevice(), 3,
        //DXGI_FORMAT_R8G8B8A8_UNORM, heaps,
        DXGI_FORMAT_B8G8R8A8_UNORM, heaps,
        heaps->GetCPUDescriptorHandleForHeapStart(),
        heaps->GetGPUDescriptorHandleForHeapStart());
    
    return true;
}

bool ToolMainLoop::LoadResources(const wstring& resPath)
{
    AddRenderItem(imguiRender.get());

    return true;
}

bool ToolMainLoop::SetDatas(IGetValue* getValue)
{
    return true;
}

void ToolMainLoop::Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    PIXEndEvent();
}

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void ToolMainLoop::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
}

void ToolMainLoop::CleanUp()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}