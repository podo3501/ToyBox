#include "pch.h"
#include "UIFixture.h"
#include "IRenderer.h"
#include "Shared/Utils/Profiler.h"
#include "Shared/Window/Window.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/Input.h"
#include "Shared/Framework/Environment.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIModule.h"

#if defined(DEBUG) | defined(_DEBUG)
void ReportLiveObjects()
{
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	if (dxgidebugdll == NULL) return;

	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	if (GetDebugInterface == nullptr) return;

	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));

	OutputDebugStringW(L"---------------Starting Live Direct3D Object Dump:----------------\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugStringW(L"---------------Completed Live Direct3D Object Dump----------------\r\n");

	debug->Release();
}
#endif

UIFixture::~UIFixture() = default;
UIFixture::UIFixture() :
	m_main{ nullptr }
{
}

void UIFixture::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	m_window = std::make_unique<Window>();

	HWND hwnd{ 0 };
	RECT rc{ 0, 0, 800, 600 };
	EXPECT_TRUE(m_window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));
	const auto& outputSize = m_window->GetOutputSize();
	InitializeEnvironment(L"Resources/", outputSize);
	m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), true);
	Input::Initialize(hwnd);

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	m_uiModule = CreateUIModule(layout, "Main", m_renderer.get(), srcBinderFilename);
	m_uiModule->AddRenderer();
	m_main = m_uiModule->GetMainPanel();

	TracyStartupProfiler();
}

void UIFixture::TearDown()
{
	TracyShutdownProfiler();

	MockMouseInput(-1, -1, false); //Ű����, ���콺�� stataic Ŭ���� �̱� ������ �����͸� �ʱ�ȭ ��Ų��.
	//�޸� �� ���� �����ش�. ������ ����� ������ ���� ������ �ʾƼ� �޸𸮰� ���� �ִµ�
	//ReportLiveObjects �Լ��� �޸𸮰� �� ���� ���ٰ� �޼����� ���� �����̴�.
	m_uiModule.reset();
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

TextureResourceBinder* UIFixture::GetResBinder() const noexcept 
{ 
	return m_uiModule->GetTexResBinder(); 
}

void UIFixture::MockMouseInput(int mouseX, int mouseY, bool leftButton)
{ //���콺�� ���°��� ������Ʈ�� ����ص� ������ ���°��� ��� ����.
	auto& mouseTracker = const_cast<MouseTracker&>(Input::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}