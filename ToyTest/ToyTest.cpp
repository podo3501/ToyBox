#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Window.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Button.h"
#include "../Toy/UserInterface/Dialog.h"

using ::testing::_;
using ::testing::Invoke;

void TestCenterRender(size_t index, const RECT& dest, const RECT* source)
{
	if (index == 0) EXPECT_EQ(dest.left, 337.0f);
	if (index == 3) EXPECT_EQ(dest.left, 361.0f);
	if (index == 6) EXPECT_EQ(dest.left, 439.0f);
}

void TestLeftTopRender(size_t index, const RECT& dest, const RECT* source)
{
	if (index == 2) EXPECT_EQ(dest.left, 400.0f);
	if (index == 5) EXPECT_EQ(dest.left, 424.0f);
	if (index == 8) EXPECT_EQ(dest.left, 502.0f);
}

TEST_F(ToyTest, ButtonTest)
{
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/");
	vector<wstring> left{ 
		L"UI/Blue/bar_square_large_l.png",
		L"UI/Red/bar_square_large_l.png",
		L"UI/Gray/bar_square_large_l.png" };
	vector<wstring> middle{
		L"UI/Blue/bar_square_large_m.png",
		L"UI/Red/bar_square_large_m.png",
		L"UI/Gray/bar_square_large_m.png" };
	vector<wstring> right{
		L"UI/Blue/bar_square_large_r.png",
		L"UI/Red/bar_square_large_r.png",
		L"UI/Gray/bar_square_large_r.png" };
	Rectangle area{ 0, 0, 126, 48 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	button->SetImage(left, middle, right, area, pos, Origin::Center);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal 버튼일 경우
	Mouse::State mouseState;
	mouseState.x = 100;
	mouseState.y = 100;
	button->Update(m_window->GetOutputSize(), mouseState);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCenterRender));
	button->Render(&mockRender);

	//clicked 버튼일 경우
	mouseState.x = 420;
	mouseState.y = 320;
	mouseState.rightButton = true;
	button->Update(m_window->GetOutputSize(), mouseState);

	//정렬을 왼쪽위로 옮긴다.
	button->ChangeOrigin(Origin::LeftTop);
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestLeftTopRender));
	button->Render(&mockRender);
}

TEST_F(ToyTest, DialogTest)
{
	unique_ptr<Dialog> dialog = make_unique<Dialog>(L"Resources/");
	Rectangle area{ 0, 0, 200, 150 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	dialog->SetImage(L"button_square_header_large_square_screws.png", area, pos, Origin::Center);
	m_renderer->AddRenderItem(dialog.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	dialog->Update(m_window->GetOutputSize());
}

//여러번 실행해서 오동작이 나는지 확인한다.
TEST(MainLoop, MultipleAppExcute)
{
	for (auto i : std::views::iota(0, 5))
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}

	std::map<int, int> test;
	std::ranges::for_each(test, [](auto& iter) {
		});
}

TEST(MainLoop, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}