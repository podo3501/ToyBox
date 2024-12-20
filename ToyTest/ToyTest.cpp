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

	void TestPanelRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		if (dest.left == 60 && dest.top == 55) EXPECT_TRUE(dest.right == 90 && dest.bottom == 91);
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

	//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
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

	//�׽�Ʈ������ windowâ�� ��� �� �ִ�. ������ App ������Ʈ���� ���� �����ϱ� ������ �ּ�ó��
	//TEST(MainLoop, RunTest)
	//{
	//	GameMainLoop mainLoop;
	//	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	//	EXPECT_EQ(mainLoop.Run(), 0);
	//}
}
