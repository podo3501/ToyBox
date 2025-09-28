#include "pch.h"
#include "../Include/IRenderer.h"
#include "GameMainLoop.h"
#include "WindowProcedure.h"
#include "Window.h"

namespace z_Core
{
	//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
	//�� �׽�Ʈ�� ������ ����� �ϴ� ���̱� ������ ��ҿ��� �� �׽�Ʈ�� ���� �ʴ´�.
	//��ҿ��� DISABLED�� �����ϰ� �ѹ��� ������ ����� �׽�Ʈ �ؾ� �� ��쿡�� DISABLED_�� �����Ѵ�.
	TEST(DISABLED_App, MultipleAppExcute)
	{
		for (auto i : std::views::iota(0, 5))
		{
			auto result = XMVerifyCPUSupport();
			EXPECT_TRUE(result);

			Window window;
			HWND hwnd{};
			result = window.Create(GetModuleHandle(nullptr), SW_HIDE, { 0, 0, 800, 600 }, hwnd);
			EXPECT_TRUE(result);

			bool bImgui = true;
			if (bImgui)
				window.AddWndProcListener([](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
				return ImguiWndProc(wnd, msg, wp, lp); });

			const auto& outputSize = window.GetOutputSize();
			unique_ptr<IRenderer> renderer = CreateRenderer(hwnd,
				static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), bImgui);
			EXPECT_TRUE(renderer != nullptr);

			unique_ptr<MainLoop> mainLoop = make_unique<GameMainLoop>(&window, renderer.get());
			EXPECT_TRUE(mainLoop->Initialize(L"Resources/", outputSize));
			mainLoop.reset();
		}
	}
}
