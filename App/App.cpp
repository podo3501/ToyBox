#include "pch.h"
#include "../Toy/GameMainLoop.h"
#include "../Toy/Window.h"
#include "../Toy/WindowProcedure.h"
#include "../Include/IRenderer.h"

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

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	int nResult = { 0 };
	//ToDo: 여기에 필요한 것을 초기화
	//괄호로 감싼 이유는 MainLoop의 스마트 포인터 소멸자를 호출해 주기 위해서이며,
	//DirectX 관련 리소스가 잘 소멸됐는지 ReportLiveObjects함수로 확인하기 때문이다.
	{
		auto result = XMVerifyCPUSupport();
		if (not result)
			return 1;

		Window window;
		HWND hwnd{};
		result = window.Create(hInstance, nShowCmd, { 0, 0, 800, 600 }, hwnd);
		if (not result)
			return 1;

		bool bImgui = false;
		if(bImgui)
			window.AddWndProcListener([](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
				return ImguiWndProc(wnd, msg, wp, lp); });

		const auto& outputSize = window.GetOutputSize();
		unique_ptr<IRenderer> renderer = CreateRenderer(hwnd, 
			static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), bImgui);
		if (not renderer)
			return 1;

		unique_ptr<MainLoop> mainLoop = make_unique<GameMainLoop>(&window, renderer.get());
		result = mainLoop->Initialize(L"../Resources/", outputSize);
		if (not result)
			return 1;

		nResult = mainLoop->Run();
		mainLoop.reset();
	}

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif

	return nResult;
}