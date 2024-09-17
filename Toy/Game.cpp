#include "pch.h"
#include "Game.h"
#include "Utility.h"
#include "Window.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

class Texture
{
public:
    Texture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap) :
        m_device{ device }, m_descHeap{ descHeap } {}
    void Upload(ResourceUploadBatch& resUpload, std::uint32_t descHeapIdx, const std::wstring& filename)
    {
        DX::ThrowIfFailed(
            CreateWICTextureFromFile(m_device, resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));
        CreateShaderResourceView(m_device, m_texture.Get(), m_descHeap->GetCpuHandle(descHeapIdx));
        m_descHeapIdx = descHeapIdx;
    }

    XMUINT2 GetSize()
    {
        return GetTextureSize(m_texture.Get());
    }

    void Reset()
    {
        m_texture.Reset();
    }

    void Draw(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2 screenPos)
    {
        auto size = GetTextureSize(m_texture.Get());
        spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx),
            GetTextureSize(m_texture.Get()),
            screenPos, nullptr, Colors::White, 0.f, { float(size.x / 2), float(size.y / 2) });
    }

private:
    ID3D12Device* m_device;
    DirectX::DescriptorHeap* m_descHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    std::uint32_t m_descHeapIdx{ 0 };
};

enum class ButtonState
{
    BT_Normal,
    BT_Over,
};

class Button
{
public:
    Button(ID3D12Device* device, DirectX::DescriptorHeap* descHeap) :
        m_device{ device }, m_descHeap{ descHeap } {}

    void SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over)
    {
        XMUINT2 size = normal->GetSize();

        m_origin.x = float(size.x / 2);
        m_origin.y = float(size.y / 2);

        m_textures.insert(std::make_pair(ButtonState::BT_Normal, std::move(normal)));
        m_textures.insert(std::make_pair(ButtonState::BT_Over, std::move(over)));
    }

    void Reset()
    {
        std::ranges::for_each(m_textures | std::views::values, [](auto& tex) {
            tex->Reset();
            });
    }

    void Update(int x, int y)
    {
        if ((-m_origin.x < x && x < m_origin.x) && (-m_origin.y < y && y < m_origin.y))
            m_over = true;
        else
            m_over = false;
    }

    void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::SimpleMath::Vector2& screenPos)
    {
        Texture* curTex{ nullptr };
        if (!m_over)
            curTex = m_textures[ButtonState::BT_Normal].get();
        else
            curTex = m_textures[ButtonState::BT_Over].get();

        curTex->Draw(spriteBatch, screenPos);
    }

private:
    ID3D12Device* m_device;
    DirectX::DescriptorHeap* m_descHeap;
    DirectX::SimpleMath::Vector2 m_origin{ 0, 0 };
    bool m_over{ false };

    std::map<ButtonState, std::unique_ptr<Texture>> m_textures;
};

LRESULT Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATEAPP:
        if (wParam)
            OnActivated();
        else
            OnDeactivated();
        Mouse::ProcessMessage(message, wParam, lParam);
        break;

    case WM_DISPLAYCHANGE:
        OnDisplayChange();
        break;

    case WM_MOVE:
        OnWindowMoved();
        break;

    case WM_PAINT:
        if (m_sizemove)
            Tick();
        else
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!m_minimized)
            {
                m_minimized = true;
                if (!m_suspend)
                    OnSuspending();
                m_suspend = true;
            }
        }
        else if (m_minimized)
        {
            m_minimized = false;
            if (m_suspend)
                OnResuming();
            m_suspend = false;
        }
        else if (!m_sizemove)
        {
            OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        m_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        m_sizemove = false;
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!m_suspend)
                OnSuspending();
            m_suspend = true;
            return TRUE;
        case PBT_APMRESUMESUSPEND:
            if (!m_minimized)
            {
                if (m_suspend)
                    OnResuming();
                m_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (m_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
                int width{ 0 };
                int height{ 0 };
                window->GetWindowSize(width, height);

                ShowWindow(hWnd, SW_SHOWNORMAL);
                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            m_fullscreen = !m_fullscreen;
        }
        break;

    case WM_ACTIVATE:
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        Mouse::ProcessMessage(message, wParam, lParam);
        break;
    }

    return 0;
}

Game::Game(const std::wstring& resPath) noexcept(false) : 
    m_resPath{ resPath }
{
    WICOnceInitialize();

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //   Add DX::DeviceResources::c_ReverseDepth to optimize depth buffer clears for 0 instead of 1.
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
bool Game::Initialize(HWND window, int width, int height)
{
    try
    {
        m_deviceResources->SetWindow(window, width, height);

        m_deviceResources->CreateDeviceResources();
        CreateDeviceDependentResources();

        m_deviceResources->CreateWindowSizeDependentResources();
        CreateWindowSizeDependentResources();

        m_mouse = std::make_unique<Mouse>();
        m_mouse->SetWindow(window);

        // TODO: Change the timer settings if you want something other than the default variable timestep mode.
        // e.g. for 60 FPS fixed timestep update logic, call:
        /*
        m_timer.SetFixedTimeStep(true);
        m_timer.SetTargetElapsedSeconds(1.0 / 60);
        */
    }
    catch(CException e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return false;
    }

    return true;
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

    Mouse::State state = m_mouse->GetState();
    /*if ((m_screenPos.x - m_origin.x < state.x && state.x < m_screenPos.x + m_origin.x) &&
        (m_screenPos.y - m_origin.y < state.y && state.y < m_screenPos.y + m_origin.y))
    {
        m_on = true;
    }
    else
        m_on = false;*/

    m_button->Update(state.x - static_cast<int>(m_screenPos.x), state.y - static_cast<int>(m_screenPos.y));

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.
    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(commandList);

    m_button->Render(m_spriteBatch.get(), m_screenPos);

    m_spriteBatch->End();

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
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
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
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
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
    m_resourceDescriptors = std::make_unique<DescriptorHeap>(device, Descriptors::Count);

    std::unique_ptr<Texture> m_tex1 = std::make_unique<Texture>(device, m_resourceDescriptors.get());
    std::unique_ptr<Texture> m_tex2 = std::make_unique<Texture>(device, m_resourceDescriptors.get());
    m_button = std::make_unique<Button>(device, m_resourceDescriptors.get());

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    std::wstring filename1 = m_resPath + std::wstring(L"1.png");
    std::wstring filename2 = m_resPath + std::wstring(L"2.png");
    m_tex1->Upload(resourceUpload, 0, filename1);
    m_tex2->Upload(resourceUpload, 1, filename2);

    m_button->SetTexture(std::move(m_tex1), std::move(m_tex2));

    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());

    SpriteBatchPipelineStateDescription pd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    auto viewport = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);

    auto size = m_deviceResources->GetOutputSize();
    m_screenPos.x = float(size.right) / 2.f;
    m_screenPos.y = float(size.bottom) / 2.f;
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_button->Reset();

    m_resourceDescriptors.reset();
    m_spriteBatch.reset();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
