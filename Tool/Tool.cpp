#include "pch.h"
#include "ToolMainLoop.h"

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

	int result = { 0 };
	//ToDo: ���⿡ �ʿ��� ���� �ʱ�ȭ
	//��ȣ�� ���� ������ MainLoop�� ����Ʈ ������ �Ҹ��ڸ� ȣ���� �ֱ� ���ؼ��̸�,
	//DirectX ���� ���ҽ��� �� �Ҹ�ƴ��� ReportLiveObjects�Լ��� Ȯ���ϱ� �����̴�.
	{
		unique_ptr<MainLoop> mainLoop = make_unique<ToolMainLoop>();
		auto initResult = mainLoop->Initialize(hInstance, L"../Resources/", nShowCmd);
		if (!initResult)
			return 1;

		result = mainLoop->Run();
		mainLoop.reset();
	}

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif

	return result;
}