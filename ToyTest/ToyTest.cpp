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
	EXPECT_TRUE(index == 0 || index == 1 || index == 2);

	if (index == 0) EXPECT_EQ(dest.left, 337.0f);
	if (index == 1) EXPECT_EQ(dest.left, 361.0f);
	if (index == 2) EXPECT_EQ(dest.left, 439.0f);

	if( index == 0 || index == 2)
		EXPECT_TRUE(source->right == 24 && source->bottom == 48);
}

void TestLeftTopRender(size_t index, const RECT& dest, const RECT* source)
{
	EXPECT_TRUE(index == 6 || index == 7 || index == 8);

	if (index == 6) EXPECT_EQ(dest.left, 400.0f);
	if (index == 7) EXPECT_EQ(dest.left, 424.0f);
	if (index == 8) EXPECT_EQ(dest.left, 502.0f);

	if (index == 6 || index == 8)
		EXPECT_TRUE(source->right == 24 && source->bottom == 48);
}

void SetMouse(int x, int y, Mouse::ButtonStateTracker& mouseTracker)
{
	Mouse::State mouseState;
	mouseState.x = x;
	mouseState.y = y;
	mouseTracker.Update(mouseState);
}

TEST_F(ToyTest, ButtonTest)
{
	std::unique_ptr<Button> button = std::make_unique<Button>();

	vector<ImageSource> normal
	{
		{ L"UI/Blue/bar_square_large_l.png" },
		{ L"UI/Blue/bar_square_large_m.png" },
		{ L"UI/Blue/bar_square_large_r.png" },
	};
	vector<ImageSource> hover
	{
		{ L"UI/Red/bar_square_large_l.png" },
		{ L"UI/Red/bar_square_large_m.png" },
		{ L"UI/Red/bar_square_large_r.png" },
	};
	vector<ImageSource> pressed
	{
		{ L"UI/Gray/bar_square_large_l.png" },
		{ L"UI/Gray/bar_square_large_m.png" },
		{ L"UI/Gray/bar_square_large_r.png" },
	};

	Rectangle area{ 0, 0, 116, 48 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	button->SetImage(L"Resources/", normal, hover, pressed, area, pos, Origin::Center);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal 버튼일 경우
	Mouse::ButtonStateTracker mouseTracker;
	SetMouse(100, 100, mouseTracker);
	button->ChangeArea({ 0, 0, 126,48 });
	button->Update(m_window->GetOutputSize(), mouseTracker);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCenterRender));
	button->Render(&mockRender);

	button->ChangeOrigin(Origin::LeftTop);	//정렬을 왼쪽위로 옮긴다.
	//clicked 버튼일 경우
	SetMouse(420, 320, mouseTracker);
	mouseTracker.leftButton = Mouse::ButtonStateTracker::PRESSED;
	button->Update(m_window->GetOutputSize(), mouseTracker);

	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestLeftTopRender));
	button->Render(&mockRender);
}

void TestDialogRender(size_t index, const RECT& dest, const RECT* source)
{
	if (dest.left == 300 && dest.top == 225) EXPECT_TRUE(dest.right == 330 && dest.bottom == 261);
	if (dest.left == 330 && dest.top == 225) EXPECT_TRUE(dest.right == 470 && dest.bottom == 261);
	if (dest.left == 300 && dest.top == 261) EXPECT_TRUE(dest.right == 330 && dest.bottom == 349);
	if (dest.left == 330 && dest.top == 261) EXPECT_TRUE(dest.right == 470 && dest.bottom == 349);
	if (dest.left == 470 && dest.top == 349) EXPECT_TRUE(dest.right == 500 && dest.bottom == 375);
}

TEST_F(ToyTest, DialogTest)
{
	unique_ptr<Dialog> dialog = make_unique<Dialog>();
	Rectangle area{ 0, 0, 170, 120 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	ImageSource dialogSource{
		L"UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	dialog->SetImage(L"Resources/", dialogSource, area, pos, Origin::Center);
	dialog->ChangeArea({ 0, 0, 200, 150 });
	m_renderer->AddRenderItem(dialog.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	dialog->Update(m_window->GetOutputSize());

	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestDialogRender));
	dialog->Render(&mockRender);
}

void TestCloseButtonRender(size_t index, const RECT& dest, const RECT* source)
{
	if (dest.left == 144 && dest.top == 104) EXPECT_TRUE(dest.right == 176 && dest.bottom == 136);

	EXPECT_EQ(index, 2);
}

TEST_F(ToyTest, CloseButton)
{
	std::unique_ptr<Button> button = std::make_unique<Button>();

	vector<ImageSource> normal { { L"UI/Blue/check_square_color_cross.png" } };
	vector<ImageSource> hover{ { L"UI/Blue/check_square_grey_cross.png" } };
	vector<ImageSource> pressed { { L"UI/Gray/check_square_grey_cross.png" } };

	Rectangle area{ 0, 0, 32, 32 };
	XMFLOAT2 pos{ 0.2f, 0.2f };
	button->SetImage(L"Resources/", normal, hover, pressed, area, pos, Origin::Center);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal 버튼일 경우
	Mouse::ButtonStateTracker mouseTracker;
	SetMouse(150, 110, mouseTracker);
	button->Update(m_window->GetOutputSize(), mouseTracker);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCloseButtonRender));
	button->Render(&mockRender);

	//Bookmark;
	//다이얼로그에 버튼 붙이기
	//패널 개념 넣기
	//글자 찍기
	//UI 데이터를 저장할 JSON 붙이기
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

//테스트에서도 window창을 띄울 수 있다. 하지만 App 프로젝트에서 실행 가능하기 때문에 주석처리
//TEST(MainLoop, RunTest)
//{
//	MainLoop mainLoop;
//	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
//	EXPECT_EQ(mainLoop.Run(), 0);
//}
