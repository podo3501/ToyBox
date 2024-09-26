#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Window.h"

using namespace DirectX;

//여러번 실행해서 오동작이 나는지 확인한다.
TEST_F(ToyTest, MultipleAppExcute)
{
	for(auto i : std::views::iota(0, 5))
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST_F(ToyTest, ButtonTest)
{
	int width = { 0 };
	int height = { 0 };
	m_window->GetWindowSize(width, height);
	std::unique_ptr<Button> button = std::make_unique<Button>(L"Resources/", width / 2, height / 2);
	m_renderer->SetRenderItem(button.get());

	EXPECT_TRUE(m_renderer->LoadResources());
}

struct ButtonImage
{
public:
	std::wstring left{};
	std::wstring middle{};
	std::wstring right{};
	XMUINT2 pos;
};

class Button3 : public IRenderItem
{
public:
	Button3(const std::wstring& resPath)
	{}
	virtual ~Button3() {};

	virtual void OnDeviceLost() override {}
	virtual void LoadResources(ID3D12Device* device,
		DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) override 
	{
		int a = 1;
	}
	virtual void Render(DirectX::DX12::SpriteBatch* sprite) override {}
	
	void SetImage(const ButtonImage& btnImage)
	{

	}

private:
};

TEST_F(ToyTest, Button3Test)
{
	int width = { 0 };
	int height = { 0 };
	m_window->GetWindowSize(width, height);
	std::unique_ptr<Button3> button3 = std::make_unique<Button3>(L"Resources/");
	ButtonImage btnImage{
		L"bar_square_large_l.png",
		L"bar_square_large_m.png",
		L"bar_square_large_r",
		XMUINT2(width / 2, height / 2) };

	button3->SetImage(btnImage);
	m_renderer->SetRenderItem(button3.get());

	EXPECT_TRUE(m_renderer->LoadResources());
}

TEST_F(ToyTest, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}