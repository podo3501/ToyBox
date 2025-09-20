#include "pch.h"
#include "../Include/IRenderer.h"
#include "GameMainLoop.h"
#include "WindowProcedure.h"
#include "Window.h"

namespace z_Core
{
	//여러번 실행해서 오동작이 나는지 확인한다.
	//이 테스트는 엔진을 띄워서 하는 것이기 때문에 평소에는 이 테스트를 켜지 않는다.
	//평소에는 DISABLED로 설정하고 한번씩 엔진을 띄워서 테스트 해야 할 경우에는 DISABLED_를 삭제한다.
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
