#include "pch.h"
#include "Renderer.h"
#include "DeviceResources.h"
#include "TextureIndexing.h"
#include "Imgui.h"
#include "Utility.h"

constexpr size_t SrvDescriptorHeapSize = 100;
constexpr size_t DsvDescriptorHeapSize = 50;

using namespace DirectX;

unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height, bool bUseImgui)
{
    unique_ptr<IImgui> imgui{ nullptr };
    if (bUseImgui)
        imgui = make_unique<Imgui>(hwnd);
    else
        imgui = make_unique<NullImgui>();

    unique_ptr<Renderer> renderer = make_unique<Renderer>(hwnd, width, height, move(imgui));
    auto result = renderer->Initialize();
    if (!result)
        return nullptr;

    return move(renderer);
}

Renderer::Renderer(HWND hwnd, int width, int height, unique_ptr<IImgui>&& imgui) noexcept(false) :
    m_imgui{ move(imgui) }
{
    WICOnceInitialize();

    m_deviceResources = make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
    m_deviceResources->RegisterDeviceNotify(this);

    m_deviceResources->SetWindow(hwnd, width, height);
}

Renderer::~Renderer()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
bool Renderer::Initialize()
{
    //com을 생성할때 다중쓰레드로 생성하게끔 초기화 한다. RAII이기 때문에 com을 사용할때 초기화 한다.
#ifdef __MINGW32__
    ReturnIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))
#else
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    if (FAILED(initialize)) return false;
#endif

    try
    {
        m_deviceResources->CreateDeviceResources();
        CreateDeviceDependentResources();

        m_deviceResources->CreateWindowSizeDependentResources();
        CreateWindowSizeDependentResources();
    }
    catch(CException e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return false;
    }

    auto device = m_deviceResources->GetD3DDevice();
    ReturnIfFalse(m_imgui->Initialize(device, m_srvDescriptors->Heap(), m_deviceResources->GetBackBufferFormat()));
    m_batch = make_unique<ResourceUploadBatch>(device);
    m_texIndexing = make_unique<TextureIndexing>(
        device, m_srvDescriptors.get(), m_batch.get(), m_spriteBatch.get());

    CreateRenderTexture();

    return true;
}


#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Renderer::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = make_unique<GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
    m_srvDescriptors = make_unique<DescriptorHeap>(device, SrvDescriptorHeapSize);
    m_rtvDescriptors = make_unique<DescriptorHeap>(device,
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE, DsvDescriptorHeapSize);

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());

    SpriteBatchPipelineStateDescription pd(rtState);
    m_spriteBatch = make_unique<SpriteBatch>(device, resourceUpload, pd);

    auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Renderer::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    auto viewport = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);
}

void Renderer::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_imgui->Reset();
    m_texIndexing->Reset();

    m_srvDescriptors.reset();
    m_spriteBatch.reset();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
}

void Renderer::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

D3D12_GPU_DESCRIPTOR_HANDLE g_srvHandle;
bool Renderer::CreateRenderTexture()
{
    //화면을 저장할 Texture를 만든다.
    auto device = m_deviceResources->GetD3DDevice();
    const DXGI_FORMAT& textureFormat = m_deviceResources->GetBackBufferFormat();
    const CD3DX12_HEAP_PROPERTIES renderTextureHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_RESOURCE_DESC renderTextureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        textureFormat,
        //DXGI_FORMAT_B8G8R8A8_UNORM,
        800,
        600,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    renderTextureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    D3D12_CLEAR_VALUE optClear{ textureFormat, { 0.f, 0.f, 0.f, 1.f } };
    device->CreateCommittedResource(
        &renderTextureHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &renderTextureDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &optClear,
        IID_PPV_ARGS(&m_renderTargetTexture)
    );
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptors->GetFirstCpuHandle());
    CreateRenderTargetView(device, m_renderTargetTexture.Get(), rtvHandle);

    //텍스춰를 읽기 위해서 Srv로 만든다.
    CreateShaderResourceView(device, m_renderTargetTexture.Get(), m_srvDescriptors->GetCpuHandle(98));

    g_srvHandle = m_srvDescriptors->GetGpuHandle(98);

    return true;
}

bool Renderer::LoadResources()
{
    //com을 생성할때 다중쓰레드로 생성하게끔 초기화 한다. RAII이기 때문에 com을 사용할때 초기화 한다.
#ifdef __MINGW32__
    ReturnIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))
#else
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    if (FAILED(initialize)) return false;
#endif
    //ResourceUploadBatch resourceUpload(device);

    m_batch->Begin();
    //resourceUpload.Begin();

    ReturnIfFalse(ranges::all_of(m_renderItems, [load = m_texIndexing.get()](const auto item) {
        return item->LoadResources(load);
        }));

    auto uploadResourcesFinished = m_batch->End(m_deviceResources->GetCommandQueue());
    uploadResourcesFinished.wait();

    return true;
}

#pragma region Frame Draw
// Draws the scene.
void Renderer::Draw()
{
    //m_imgui->PrepareRender();
    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    /*Clear();*/

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.
    ID3D12DescriptorHeap* heaps[] = { m_srvDescriptors->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(size(heaps)), heaps);

    // 클라이언트 화면을 렌더링할 렌더 타겟 뷰 설정
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptors->GetFirstCpuHandle());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    //// 렌더 타겟 클리어
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);
    
    m_spriteBatch->Begin(commandList);

    ranges::for_each(m_renderItems, [renderer = m_texIndexing.get()](const auto item) {
        item->Render(renderer);
        });

    m_spriteBatch->End();

    Clear();
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //ImGui::Begin("Client Window");
    ImGui::Begin("Horizontal Scrolling Example", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    // 클라이언트 텍스처 크기 지정
    ImVec2 textureSize(800.f, 600.0f);

    // 텍스처 핸들 가져오기 (SRV Heap에서 Gpu Handle 가져옴)
    
    /*D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = m_srvDescriptors->GetGpuHandle(98);*/

    // ImGui에 텍스춰에 찍어논 화면을 연결
    ImGui::Image((ImTextureID)g_srvHandle.ptr, textureSize);

    ImGui::End();

    // Rendering
    ImGui::Render();

    m_imgui->Render(commandList);

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Renderer::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Renderer::OnActivated()
{
    // TODO: Renderer is becoming active window.
}

void Renderer::OnDeactivated()
{
    // TODO: Renderer is becoming background window.
}

void Renderer::OnSuspending()
{
    // TODO: Renderer is being power-suspended (or minimized).
}

void Renderer::OnResuming()
{
    // TODO: Renderer is being power-resumed (or returning from minimize).
}

void Renderer::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Renderer::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Renderer::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Renderer window is being resized.
}
#pragma endregion

IGetValue* Renderer::GetValue() const noexcept { return m_texIndexing.get(); }
void Renderer::AddRenderItem(IRenderItem* item) { m_renderItems.emplace_back(item); }
void Renderer::AddImguiItem(IImguiItem* item) { m_imgui->AddItem(item); }