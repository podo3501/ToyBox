#include "pch.h"
#include "ToyTest.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Renderer.h"
#include "../Toy/Window.h"

using namespace DirectX;

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

void ToyTest::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void ToyTest::TearDown()
{
#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

//여러번 실행해서 오동작이 나는지 확인한다.
TEST_F(ToyTest, MultipleAppExcute)
{
	for(auto i : std::views::iota(0, 5))
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST_F(ToyTest, ButtonTest)
{
	std::unique_ptr<Window> window = std::make_unique<Window>();
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

	HWND hwnd{ 0 };
	RECT rc{};
	//hide로 고쳤을때 false가 났다.
	EXPECT_TRUE(window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));

	int width = static_cast<int>(rc.right - rc.left);
	int height = static_cast<int>(rc.bottom - rc.top);
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/", width / 2, height / 2);
	renderer->SetRenderItem(button.get());

	EXPECT_TRUE(renderer->Initialize(hwnd, width, height));
}

TEST_F(ToyTest, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}