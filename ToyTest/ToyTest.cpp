#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Window.h"

using namespace DirectX;

class MockRender : public IRender
{
public:
	virtual ~MockRender() = default;

	MOCK_METHOD(void, Render, (int index, const DirectX::SimpleMath::Vector2& position, const DirectX::XMFLOAT2& origin), (override));
};

void TestCenterRender(int index, const DirectX::SimpleMath::Vector2& position, const DirectX::XMFLOAT2& origin)
{
	if (index == 3) EXPECT_EQ(position.x, 364.0f);
	if (index == 4) EXPECT_EQ(position.x, 400.0f);
	if (index == 5) EXPECT_EQ(position.x, 436.0f);

	EXPECT_EQ(position.y, 300.0f);
}

void TestLeftTopRender(int index, const DirectX::SimpleMath::Vector2& position, const DirectX::XMFLOAT2& origin)
{
	if (index == 3) EXPECT_EQ(position.x, 364.0f);
	if (index == 4) EXPECT_EQ(position.x, 400.0f);
	if (index == 5) EXPECT_EQ(position.x, 436.0f);

	EXPECT_EQ(position.y, 300.0f);
}

using ::testing::_;
using ::testing::Invoke;

TEST_F(ToyTest, ButtonTest)
{
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/");
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
	XMUINT2 size{ 48, 48 };
	XMFLOAT2 pos{ 0.5f, 0.5f };
	button->SetImage(normal, over, clicked, size, pos, Origin::Center);
	m_renderer->AddRenderItem(button.get());
	EXPECT_TRUE(m_renderer->LoadResources());

	Mouse::State mouseState;
	mouseState.x = 100;
	mouseState.y = 100;
	button->Update(m_window->GetOutputSize(), mouseState);

	//테스트를 하려면 renderer를 인자로 넣어주어야 한다.
	//그 값들을 테스트 하고 싶다면 testRenderer를 만들어서 넘어오는 값들에 대해서 분석한다.
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(TestCenterRender));
	button->Render(&mockRender);

	//button->SetPosition{ 0.5f, 0.5f };
	//button->SetOrigin(Origin::LeftTop);
	//Mouse::State mouseState;
	//mouseState.x = 100;
	//mouseState.y = 100;
	//button->Update(m_window->GetOutputSize(), mouseState);
	//EXPECT_CALL(mockRender, Render(_, _, _)).WillOnce(Invoke(TestLeftTopRender));
	//button->Render(&mockRender);
}

TEST_F(ToyTest, DialogTest)
{

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