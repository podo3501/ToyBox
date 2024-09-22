#include "pch.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Renderer.h"
#include "../Toy/Window.h"

using namespace DirectX;

//여러번 실행해서 오동작이 나는지 확인한다.
TEST(MainLoop, MultipleInitializeTest)
{
	for(int i = 1; i < 5; ++i)
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST(MainLoop, ButtonTest)
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

TEST(MainLoop, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}