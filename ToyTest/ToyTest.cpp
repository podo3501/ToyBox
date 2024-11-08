#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "../Toy/GameMainLoop.h"
#include "../Toy/WindowProcedure.h"
#include "../Toy/Window.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Button.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/TextArea.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/UserInterface/Scene.h"

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

	UILayout layout({ 0, 0, 116, 48 }, { 0.0f, 0.0f }, Origin::Center);
	button->SetImage(m_renderer.get(), { 0.5f, 0.5f }, layout, normal, hover, pressed);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal 버튼일 경우
	Mouse::ButtonStateTracker mouseTracker;
	SetMouse(100, 100, mouseTracker);
	button->ChangeArea({ 0, 0, 126,48 });
	button->Update({ 0.f, 0.f }, &mouseTracker);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCenterRender));
	button->Render(&mockRender);

	button->ChangeOrigin(Origin::LeftTop);	//정렬을 왼쪽위로 옮긴다.
	//clicked 버튼일 경우
	SetMouse(420, 320, mouseTracker);
	mouseTracker.leftButton = Mouse::ButtonStateTracker::PRESSED;
	button->Update({ 0.f, 0.f }, &mouseTracker);

	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestLeftTopRender));
	button->Render(&mockRender);
}

void TestBGImageRender(size_t index, const RECT& dest, const RECT* source)
{
	if (dest.left == 300 && dest.top == 225) EXPECT_TRUE(dest.right == 330 && dest.bottom == 261);
	if (dest.left == 330 && dest.top == 225) EXPECT_TRUE(dest.right == 470 && dest.bottom == 261);
	if (dest.left == 300 && dest.top == 261) EXPECT_TRUE(dest.right == 330 && dest.bottom == 349);
	if (dest.left == 330 && dest.top == 261) EXPECT_TRUE(dest.right == 470 && dest.bottom == 349);
	if (dest.left == 470 && dest.top == 349) EXPECT_TRUE(dest.right == 500 && dest.bottom == 375);
}

TEST_F(ToyTest, BGImageTest)
{
	unique_ptr<BGImage> bgImage = make_unique<BGImage>();
	ImageSource bgImageSource{
		L"UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	UILayout layout({ 0, 0, 170, 120 }, { 0.0f, 0.0f }, Origin::Center);
	bgImage->SetImage(m_renderer.get(), { 0.f, 0.f }, layout, bgImageSource);
	bgImage->ChangeArea({ 0, 0, 200, 150 });
	m_renderer->AddRenderItem(bgImage.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	bgImage->Update({0.f, 0.f});

	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestBGImageRender));
	bgImage->Render(&mockRender);
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

	UILayout layout({ 0, 0, 32, 32 }, { 0.0f, 0.0f }, Origin::Center);
	button->SetImage(m_renderer.get(), { 0.2f, 0.2f }, layout, normal, hover, pressed);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal 버튼일 경우
	Mouse::ButtonStateTracker mouseTracker;
	SetMouse(150, 110, mouseTracker);
	button->Update({0.f, 0.f}, &mouseTracker);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCloseButtonRender));
	button->Render(&mockRender);
}

void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
{
	if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
	if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
	if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
	if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
}

TEST_F(ToyTest, TextArea)
{
	std::unique_ptr<TextArea> textArea = std::make_unique<TextArea>();
	UILayout layout({ 0, 0, 320, 120 }, { 0.0f, 0.0f }, Origin::Center);
	map<wstring, wstring> fontFileList;
	fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
	fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
	textArea->SetFont(m_renderer.get(), { 0.5f, 0.5f }, layout, fontFileList);
	m_renderer->AddRenderItem(textArea.get());

	m_renderer->LoadResources();

	textArea->SetText(m_renderer->GetValue(),
		L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>");

	textArea->Update({ 0.f, 0.f });

	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(TestTextAreaRender));
	textArea->Render(&mockRender);
}

TEST_F(ToyTest, Panel)
{
	unique_ptr<Panel> panel = std::make_unique<Panel>();
	Rectangle area{ 0, 0, 220, 190 };
	UILayout layout(area, { 0.0f, 0.0f }, Origin::LeftTop);
	ImageSource dialogSource{
		L"Resources/UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	unique_ptr<BGImage> bgImg = make_unique<BGImage>();
	bgImg->SetImage(m_renderer.get(), { 0.f, 0.f }, layout, dialogSource);
	panel->AddRenderItem({ 0.1f, 0.1f }, move(bgImg));

	EXPECT_EQ(area, panel->GetArea());
}

TEST_F(ToyTest, Dialog)
{
	unique_ptr<Dialog> dialog = std::make_unique<Dialog>();
	dialog->SetUIItem();

	m_renderer->AddRenderItem(dialog.get());
	EXPECT_TRUE(m_renderer->LoadResources());
}

TEST_F(ToyTest, Scene)
{
	unique_ptr<Scene>testScene = std::make_unique<Scene>(m_renderer.get());
	EXPECT_TRUE(testScene->LoadData(L"UI/Data/FirstScene.json"));

	m_renderer->AddLoadScene(testScene.get());
	m_renderer->AddRenderScene(testScene.get());

	EXPECT_TRUE(m_renderer->LoadScenes());
}

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
