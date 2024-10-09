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
	EXPECT_TRUE(index == 0 || index == 3 || index == 6);

	if (index == 0) EXPECT_EQ(dest.left, 337.0f);
	if (index == 3) EXPECT_EQ(dest.left, 361.0f);
	if (index == 6) EXPECT_EQ(dest.left, 439.0f);

	if( index == 0 || index == 6)
		EXPECT_TRUE(source->right == 24 && source->bottom == 48);
}

void TestLeftTopRender(size_t index, const RECT& dest, const RECT* source)
{
	EXPECT_TRUE(index == 2 || index == 5 || index == 8);

	if (index == 2) EXPECT_EQ(dest.left, 400.0f);
	if (index == 5) EXPECT_EQ(dest.left, 424.0f);
	if (index == 8) EXPECT_EQ(dest.left, 502.0f);

	if (index == 2 || index == 8)
		EXPECT_TRUE(source->right == 24 && source->bottom == 48);
}

TEST_F(ToyTest, ButtonTest)
{
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/");

	vector<ImageSource> left
	{
		{ L"UI/Blue/bar_square_large_l.png", { { 0, 0, 24, 48 } } },
		{ L"UI/Red/bar_square_large_l.png", { {} } },
		{ L"UI/Gray/bar_square_large_l.png", { {} } }
	};
	vector<ImageSource> middle
	{
		{ L"UI/Blue/bar_square_large_m.png", { {} } },
		{ L"UI/Red/bar_square_large_m.png", { { 0, 0, 48, 48 } } },
		{ L"UI/Gray/bar_square_large_m.png", { {} } }
	};
	vector<ImageSource> right
	{
		{ L"UI/Blue/bar_square_large_r.png", { {} } },
		{ L"UI/Red/bar_square_large_r.png", { {} } },
		{ L"UI/Gray/bar_square_large_r.png", { { 0, 0, 24, 48 } } }
	};

	Rectangle area{ 0, 0, 126, 48 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	button->SetImage(left, middle, right, area, pos, Origin::Center);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	//normal ��ư�� ���
	Mouse::State mouseState;
	Mouse::ButtonStateTracker mouseTracker;
	mouseState.x = 100;
	mouseState.y = 100;
	mouseTracker.Update(mouseState);
	button->Update(m_window->GetOutputSize(), mouseTracker);

	//�׽�Ʈ�� �Ϸ��� renderer�� ���ڷ� �־��־�� �Ѵ�.
	//�� ������ �׽�Ʈ �ϰ� �ʹٸ� testRenderer�� ���� �Ѿ���� ���鿡 ���ؼ� �м��Ѵ�.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCenterRender));
	button->Render(&mockRender);

	//clicked ��ư�� ���
	mouseState.x = 420;
	mouseState.y = 320;
	mouseTracker.Update(mouseState);
	mouseTracker.leftButton = Mouse::ButtonStateTracker::PRESSED;
	button->Update(m_window->GetOutputSize(), mouseTracker);

	//������ �������� �ű��.
	button->ChangeOrigin(Origin::LeftTop);
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestLeftTopRender));
	button->Render(&mockRender);
	
	Bookmark;
	//UI ���̾�α׸� �����.
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

//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
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

//�׽�Ʈ������ windowâ�� ��� �� �ִ�. ������ App ������Ʈ���� ���� �����ϱ� ������ �ּ�ó��
//TEST(MainLoop, RunTest)
//{
//	MainLoop mainLoop;
//	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
//	EXPECT_EQ(mainLoop.Run(), 0);
//}

TEST_F(ToyTest, MemoryTest)
{

}