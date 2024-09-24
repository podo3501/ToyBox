#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Window.h"

using namespace DirectX;

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
	std::unique_ptr<IRenderer> renderer = CreateRenderer();

	HWND hwnd{ 0 };
	RECT rc{};
	
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