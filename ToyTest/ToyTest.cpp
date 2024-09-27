#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Button3.h"
#include "../Toy/Window.h"
#include "../Toy/Texture.h"

using namespace DirectX;

TEST_F(ToyTest, ButtonTest)
{
	int width = { 0 };
	int height = { 0 };
	m_window->GetWindowSize(width, height);
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/", width / 2, height / 2);
	m_renderer->SetRenderItem(button.get());

	EXPECT_TRUE(m_renderer->LoadResources());
}

TEST_F(ToyTest, Button3Test)
{
	int width = { 0 };
	int height = { 0 };
	m_window->GetWindowSize(width, height);
	std::unique_ptr<Button3> button3 = std::make_unique<Button3>(L"Resources/");
	ButtonImage btnImage{ 3, { 
			L"bar_square_large_l.png", 
			L"bar_square_large_m.png", 
			L"bar_square_large_r.png" 
	} };

	button3->SetImage(btnImage, XMUINT2(width / 2, height / 2));
	m_renderer->SetRenderItem(button3.get());

	EXPECT_TRUE(m_renderer->LoadResources());
}

//여러번 실행해서 오동작이 나는지 확인한다.
TEST(MainLoop, MultipleAppExcute)
{
	for (auto i : std::views::iota(0, 5))
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST(MainLoop, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}