#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "../Toy/GameMainLoop.h"
#include "../Toy/WindowProcedure.h"
#include "../Toy/Window.h"
#include "../Toy/Utility.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/OButton.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/TextArea.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/InputManager.h"

using ::testing::_;
using ::testing::Invoke;

namespace BasicClient
{
	void TestCenterRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0 || index == 1 || index == 2);

		if (index == 0) EXPECT_EQ(dest.left, 337.0f);
		if (index == 1) EXPECT_EQ(dest.left, 361.0f);
		if (index == 2) EXPECT_EQ(dest.left, 439.0f);

		if (index == 0 || index == 2)
			EXPECT_TRUE(source->right == 24 && source->bottom == 48);
	}

	void TestLeftTopRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 6 || index == 7 || index == 8);

		if (index == 6) EXPECT_EQ(dest.left, 400.0f);
		if (index == 7) EXPECT_EQ(dest.left, 424.0f);
		if (index == 8) EXPECT_EQ(dest.left, 502.0f);

		if (index == 6 || index == 8)
			EXPECT_TRUE(source->right == 24 && source->bottom == 48);
	}

	void SetMouse(int x, int y, InputManager& inputManager)
	{
		Mouse::State mouseState;
		mouseState.x = x;
		mouseState.y = y;
		inputManager.Update(mouseState);
	}

	TEST_F(ToyTestFixture, ButtonTest)
	{
		vector<ImageSource> normal {
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

		UILayout layout({ 0, 0, 116, 48 }, Origin::Center);
		unique_ptr<OButton> button = make_unique<OButton>();
		button->SetImage("Button", layout, normal, hover, pressed);
		button->ChangeArea({ 0, 0, 126,48 });

		m_panel->AddComponent(move(button), { 0.5f, 0.5f });
		m_renderer->LoadComponents();
		
		//normal 버튼일 경우
		InputManager inputManager(m_window->GetHandle());
		SetMouse(100, 100, inputManager);
		m_panel->Update({}, &inputManager);

		//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
		//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
		CallMockRender(m_panel.get(), TestCenterRender);

		UIComponent* btn = m_panel->GetComponent("Button");
		btn->ChangeOrigin(Origin::LeftTop);	//정렬을 왼쪽위로 옮긴다.
		SetMouse(420, 320, inputManager);
		inputManager.GetMouse()->leftButton = Mouse::ButtonStateTracker::PRESSED;	//clicked 버튼일 경우
		m_panel->Update({}, &inputManager);

		CallMockRender(m_panel.get(), TestLeftTopRender);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	void TestBGImageRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		if (dest.left == 300 && dest.top == 225) EXPECT_TRUE(dest.right == 330 && dest.bottom == 261);
		if (dest.left == 330 && dest.top == 225) EXPECT_TRUE(dest.right == 470 && dest.bottom == 261);
		if (dest.left == 300 && dest.top == 261) EXPECT_TRUE(dest.right == 330 && dest.bottom == 349);
		if (dest.left == 330 && dest.top == 261) EXPECT_TRUE(dest.right == 470 && dest.bottom == 349);
		if (dest.left == 470 && dest.top == 349) EXPECT_TRUE(dest.right == 500 && dest.bottom == 375);
	}

	unique_ptr<UIComponent> CreateTestBGImage(IRenderer* renderer, const string& name, const Rectangle& area)
	{
		UILayout layout(area, Origin::LeftTop);
		ImageSource dialogSource{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
				{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
				{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
			}
		};
		unique_ptr<BGImage> bgImg = make_unique<BGImage>();
		bgImg->SetImage(name, layout, dialogSource);

		return move(bgImg);
	}

	TEST_F(ToyTestFixture, BGImageTest)
	{
		unique_ptr<UIComponent> bgImage = CreateTestBGImage(m_renderer.get(), "BGImage", { 0, 0, 170, 120 });
		m_renderer->AddComponent(bgImage.get(), true);

		bgImage->ChangeArea({ 0, 0, 200, 150 });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(bgImage.get(), TestBGImageRender);

		EXPECT_TRUE(WriteReadTest(bgImage));
	}

	void TestCloseButtonRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		if (dest.left == 144 && dest.top == 104) EXPECT_TRUE(dest.right == 176 && dest.bottom == 136);

		EXPECT_EQ(index, 2);
	}

	TEST_F(ToyTestFixture, CloseButton)
	{
		std::unique_ptr<OButton> button = std::make_unique<OButton>();

		vector<ImageSource> normal{ { L"UI/Blue/check_square_color_cross.png" } };
		vector<ImageSource> hover{ { L"UI/Blue/check_square_grey_cross.png" } };
		vector<ImageSource> pressed{ { L"UI/Gray/check_square_grey_cross.png" } };

		UILayout layout({ 0, 0, 32, 32 }, Origin::Center);
		button->SetImage("Button", layout, normal, hover, pressed);

		m_panel->AddComponent(move(button), { 0.2f, 0.2f });
		m_renderer->LoadComponents();

		//normal 버튼일 경우
		InputManager inputManager(m_window->GetHandle());
		SetMouse(150, 110, inputManager);
		m_panel->Update({}, &inputManager);

		//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
		//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
		CallMockRender(m_panel.get(), TestCloseButtonRender);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
	{
		if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
		if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
	}

	TEST_F(ToyTestFixture, TextArea)
	{
		std::unique_ptr<TextArea> cTextArea = std::make_unique<TextArea>();
		UILayout layout({ 0, 0, 320, 120 }, Origin::Center);
		map<wstring, wstring> fontFileList;
		fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
		fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
		wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
		cTextArea->SetFont("TextArea", text, layout, fontFileList);

		m_panel->AddComponent(move(cTextArea), { 0.5f, 0.5f });
		m_renderer->LoadComponents();

		MockRender mockRender;
		EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(TestTextAreaRender));
		m_panel->Render(&mockRender);

		//clone test
		TextArea* textArea = static_cast<TextArea*>(m_panel->GetComponent("TextArea"));
		std::unique_ptr<UIComponent> rTextArea = textArea->Clone();
		rTextArea->SetName("rTextArea");
		TextArea& writeComp = *textArea;
		writeComp.AddComponent(move(rTextArea), { 0.03f, 0.04f });
		std::unique_ptr<UIComponent> emptyTextArea = make_unique<TextArea>();
		emptyTextArea->SetName("emptyTextArea");
		writeComp.AddComponent(move(emptyTextArea), { 0.3f, 0.4f });

		EXPECT_TRUE(WriteReadTest(m_panel));
		////클론 했을때 이름 바꾸는 것은 Clone 함수 안에서 바꾸도록 수정
	}

	void TestPanelRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		if (dest.left == 60 && dest.top == 55) EXPECT_TRUE(dest.right == 90 && dest.bottom == 91);
	}

	TEST_F(ToyTestFixture, Panel)
	{
		m_panel->ChangeArea({ 100, 100, 700, 500 });
		unique_ptr<UIComponent> bgImg = CreateTestBGImage(m_renderer.get(), "BGImage", { 0, 0, 220, 190 });
		bgImg->GetLayout()->Set(Origin::Center);

		Rectangle bgArea = bgImg->GetArea();
		m_panel->AddComponent(move(bgImg), { 0.1f, 0.1f });

		Rectangle panelArea = m_panel->GetArea();
		m_renderer->LoadComponents();

		CallMockRender(m_panel.get(), TestPanelRender);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	TEST_F(ToyTestFixture, Dialog)
	{
		unique_ptr<UIComponent> dialog = std::make_unique<Dialog>();
		dialog->SetName("Dialog");
		unique_ptr<UIComponent> bgImg = CreateTestBGImage(m_renderer.get(), "BGImage", { 0, 0, 220, 190 });
		dialog->AddComponent(move(bgImg), { 0.0f, 0.1f });

		unique_ptr<UIComponent> cloneDialog = dialog->Clone();

		m_panel->AddComponent(move(dialog), {});
		m_panel->AddComponent(move(cloneDialog), { 0.f, 0.4f });
		m_renderer->LoadComponents();

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	TEST_F(ToyTestFixture, GetComponents)
	{
		unique_ptr<UIComponent> bgImg = CreateTestBGImage(m_renderer.get(), "BGImage", { 0, 0, 220, 190 });
		m_panel->AddComponent(move(bgImg), { 0.1f, 0.1f });

		vector<UIComponent*> componentList1;
		m_panel->GetComponents({ 0, 0 }, componentList1);
		EXPECT_EQ(componentList1.size(), 1);

		vector<UIComponent*> componentList2;
		m_panel->GetComponents({ 100, 100 }, componentList2);
		EXPECT_EQ(componentList2.size(), 2);
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
}
