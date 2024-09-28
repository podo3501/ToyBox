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
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/", SimpleMath::Vector2{ 0.5f, 0.5f });
	m_renderer->SetRenderItem(button.get());

	EXPECT_TRUE(m_renderer->LoadResources());
}

TEST_F(ToyTest, Button3Test)
{
	std::unique_ptr<Button3> button3 = std::make_unique<Button3>(L"Resources/");
	ButtonImage btnImage{ 3, { 
			L"UI/Blue/bar_square_large_l.png", 
			L"UI/Blue/bar_square_large_m.png", 
			L"UI/Blue/bar_square_large_r.png" 
	} };

	SimpleMath::Vector2 pos{ 0.5f, 0.5f };
	button3->SetImage(btnImage, pos);
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