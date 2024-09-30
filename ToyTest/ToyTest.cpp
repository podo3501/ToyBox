#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Button3.h"
#include "../Toy/Window.h"
#include "../Toy/Texture.h"

using namespace DirectX;

TEST_F(ToyTest, Button3Test)
{
	std::unique_ptr<Button3> button3 = std::make_unique<Button3>(L"Resources/");
	ButtonImage normal{ 3, { 
			L"UI/Blue/bar_square_large_l.png", 
			L"UI/Blue/bar_square_large_m.png", 
			L"UI/Blue/bar_square_large_r.png" 
	} };
	ButtonImage over{ 6, {
			L"UI/Red/bar_square_large_l.png",
			L"UI/Red/bar_square_large_m.png",
			L"UI/Red/bar_square_large_r.png"
	} };
	ButtonImage clicked{ 9, {
			L"UI/Gray/bar_square_large_l.png",
			L"UI/Gray/bar_square_large_m.png",
			L"UI/Gray/bar_square_large_r.png"
	} };
	SimpleMath::Vector2 pos{ 0.5f, 0.5f };
	button3->SetImage(normal, over, clicked, pos);
	button3->LoadResources(m_renderer.get());

	Mouse::State mouseState;
	mouseState.x = 100;
	mouseState.y = 100;
	button3->Update(m_window->GetOutputSize(), mouseState);
	//�׽�Ʈ�� �Ϸ��� renderer�� ���ڷ� �־��־�� �Ѵ�.
	//testRenderer���� �޴� ���� ��ġ�� ���� ���� �ް� directx�� ���� �͵��� renderer�� �ִ´�.
	//�ؽ��ĸ� �ε� �ؼ� ���� ���� �����ؼ� �� ������ �׽�Ʈ �ϰ� �ʹٸ� testRenderer�� ����
	//�Ѿ���� ���鿡 ���ؼ� �м��Ѵ�.
	//button3->Draw(testRenderer);	
}

//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
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