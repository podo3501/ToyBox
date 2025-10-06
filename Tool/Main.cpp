#include "pch.h"
#include "ToolLoop.h"
#include "Shared/Framework/Initializer/Application.h"
#include "Core/Utils/DxLeakCheck.h"

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

	//��ȣ�� ���� ������ MainLoop�� ����Ʈ ������ �Ҹ��ڸ� ȣ���� �ֱ� ���ؼ��̸�,
	//DirectX ���� ���ҽ��� �� �Ҹ�ƴ��� ReportLiveObjects�Լ��� Ȯ���ϱ� �����̴�.
	int nResult = { 0 };
	{
		RECT windowRect = { 0, 0, 1280, 960 };
		std::wstring resourcePath = L"../Resources/";
		auto toolLoop = CreateAppLoop<ToolLoop>(hInstance, nShowCmd, windowRect, resourcePath, true);
		if (!toolLoop)
			return 1;

		nResult = toolLoop->Run();
		toolLoop.reset();
	}

#if defined(DEBUG) | defined(_DEBUG)
	CheckDirectxLeaks();
#endif

	return nResult;
}