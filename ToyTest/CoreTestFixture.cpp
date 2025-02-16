#include "pch.h"
#include "CoreTestFixture.h"
#include "../Toy/Window.h"
#include "../Core/DeviceResources.h"
#include "Utility.h"

CoreTest::CoreTest() = default;

CoreTest::~CoreTest()
{
	if (m_deviceResources)
		m_deviceResources->WaitForGpu();

	m_spriteBatch.reset();
	m_batch.reset();
	m_resourceDescriptors.reset();
	m_graphicsMemory.reset();
	m_deviceResources.reset();
	m_window.reset();
	
#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

void CoreTest::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	m_window = std::make_unique<Window>();

	HWND hwnd{ 0 };
	RECT rc{ 0, 0, 800, 600 };
	EXPECT_TRUE(m_window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));
	const auto& outputSize = m_window->GetOutputSize();

	m_deviceResources = make_unique<DX::DeviceResources>();
	m_deviceResources->SetWindow(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y));

	EXPECT_TRUE(Initialize());
}

void CoreTest::TearDown()
{}

bool CoreTest::Initialize()
{
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	auto device = m_deviceResources->GetD3DDevice();
	m_batch = make_unique<ResourceUploadBatch>(device);

	return true;
}

void CoreTest::CreateDeviceDependentResources()
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
	m_resourceDescriptors = make_unique<DescriptorHeap>(device, 100);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());

	SpriteBatchPipelineStateDescription pd(rtState);
	m_spriteBatch = make_unique<SpriteBatch>(device, resourceUpload, pd);

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();
}

void CoreTest::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.
	auto viewport = m_deviceResources->GetScreenViewport();
	m_spriteBatch->SetViewport(viewport);
}

bool CoreTest::LoadResources(function<bool(ITextureLoad*)> funcLoad, ITextureLoad* load)
{
	//com을 생성할때 다중쓰레드로 생성하게끔 초기화 한다. RAII이기 때문에 com을 사용할때 초기화 한다.
#ifdef __MINGW32__
	ReturnIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))
#else
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize)) return false;
#endif

	m_batch->Begin();

	ReturnIfFalse(funcLoad(load));
	
	auto uploadResourcesFinished = m_batch->End(m_deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();

	return true;
}