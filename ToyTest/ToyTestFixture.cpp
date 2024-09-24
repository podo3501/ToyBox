#include "pch.h"
#include "ToyTestFixture.h"

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