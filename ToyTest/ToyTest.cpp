#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "../Toy/GameMainLoop.h"
#include "../Toy/WindowProcedure.h"
#include "../Toy/Window.h"

namespace BasicClient
{
	//여러번 실행해서 오동작이 나는지 확인한다.
	TEST(MainLoop, MultipleAppExcute)
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

	//테스트에서도 window창을 띄울 수 있다. 하지만 App 프로젝트에서 실행 가능하기 때문에 주석처리
	//TEST(MainLoop, RunTest)
	//{
	//	GameMainLoop mainLoop;
	//	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	//	EXPECT_EQ(mainLoop.Run(), 0);
	//}
}
