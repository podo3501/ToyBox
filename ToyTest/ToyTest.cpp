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
#include "../Toy/UserInterface/TextArea.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/ImageGrid9.h"

using ::testing::_;
using ::testing::Invoke;

namespace BasicClient
{
	unique_ptr<UIComponent> CreateTestImageGrid9(IRenderer* renderer, const string& name, const Rectangle& area)
	{
		UILayout layout(area, Origin::LeftTop);
		ImageSource source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
				{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
				{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
			}
		};
		unique_ptr<ImageGrid9> img9 = make_unique<ImageGrid9>();
		img9->SetImage(name, layout, source);

		return move(img9);
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
		m_panel->ProcessRender(&mockRender);

		////clone test
		//TextArea* textArea = static_cast<TextArea*>(m_panel->GetComponent("TextArea"));
		//std::unique_ptr<UIComponent> rTextArea = textArea->Clone();
		//rTextArea->SetName("rTextArea");
		//TextArea& writeComp = *textArea;
		//writeComp.AddComponent(move(rTextArea), { 0.03f, 0.04f });
		//std::unique_ptr<UIComponent> emptyTextArea = make_unique<TextArea>();
		//emptyTextArea->SetName("emptyTextArea");
		//writeComp.AddComponent(move(emptyTextArea), { 0.3f, 0.4f });

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
		unique_ptr<UIComponent> bgImg = CreateTestImageGrid9(m_renderer.get(), "ImageGrid9", { 0, 0, 220, 190 });
		bgImg->GetLayout()->Set(Origin::Center);

		Rectangle bgArea = bgImg->GetArea();
		m_panel->AddComponent(move(bgImg), { 0.1f, 0.1f });

		Rectangle panelArea = m_panel->GetArea();
		m_renderer->LoadComponents();

		CallMockRender(m_panel.get(), TestPanelRender, 1);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	TEST_F(ToyTestFixture, GetComponents)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), "ImageGrid9", { 0, 0, 220, 190 });
		m_panel->AddComponent(move(img9), { 0.1f, 0.1f });

		vector<UIComponent*> componentList1;
		m_panel->GetComponents({ 0, 0 }, componentList1);
		EXPECT_EQ(componentList1.size(), 1);

		vector<UIComponent*> componentList2;
		m_panel->GetComponents({ 100, 100 }, componentList2);
		EXPECT_EQ(componentList2.size(), 4);
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
