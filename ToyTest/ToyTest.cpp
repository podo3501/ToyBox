#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "../Toy/GameMainLoop.h"
#include "../Toy/WindowProcedure.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/UserInterface/Panel.h"

namespace BasicClient
{
	unique_ptr<UIComponent> CreateTestImageGrid9(IRenderer* renderer, const string& name, const XMUINT2& size)
	{
		UILayout layout(size, Origin::LeftTop);
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

	TEST_F(ToyTestFixture, GetComponents)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), "ImageGrid9", { 220, 190 });
		m_panel->AddComponent(move(img9), { 0.1f, 0.1f });

		vector<UIComponent*> componentList1;
		m_panel->GetComponents({ 0, 0 }, componentList1);
		EXPECT_EQ(componentList1.size(), 1);

		vector<UIComponent*> componentList2;
		m_panel->GetComponents({ 100, 100 }, componentList2);
		EXPECT_EQ(componentList2.size(), 4);

		unique_ptr<UIComponent> img9_2 = CreateTestImageGrid9(m_renderer.get(), "ImageGrid9", { 221, 191 });
		m_panel->AddComponent(move(img9_2), { 0.11f, 0.11f });

		vector<UIComponent*> componentList3;
		m_panel->GetComponents({ 180, 160 }, componentList3);
	}

	TEST_F(ToyTestFixture, GetPosition)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), "ImageGrid9", { 220, 190 });
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		panel->AddComponent(move(img9), { 0.1f, 0.1f });
		m_panel->AddComponent(move(panel), { 0.5f, 0.5f });
		m_panel->ProcessUpdate({}, nullptr);

		UIComponent* component = m_panel->GetComponent("ImageGrid9_1_1");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));

		auto rect = component->GetRectangle();
		EXPECT_EQ(rect, Rectangle(270, 216, 160, 128));
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
