#include "pch.h"
#include "../Toy/MainLoop.h"
#include "../Toy/Button.h"
#include "../Toy/Texture.h"
#include "../Toy/DeviceResources.h"

using namespace DirectX;

//여러번 실행해서 오동작이 나는지 확인한다.
TEST(MainLoop, MultipleInitializeTest)
{
	for(int i = 1; i < 5; ++i)
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST(MainLoop, ButtonTest)
{
	std::unique_ptr<DX::DeviceResources> deviceResources = std::make_unique<DX::DeviceResources>();
	auto device = deviceResources->GetD3DDevice();
	std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors = 
		std::make_unique<DirectX::DescriptorHeap>(device, 5);

	Button button(device, resourceDescriptors.get());
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	std::wstring resPath = L"Resources/";
	std::vector<std::tuple<int, std::wstring>> filenames{
		{0, resPath + std::wstring(L"1.png")},
		{1, resPath + std::wstring(L"2.png")},
		{2, resPath + std::wstring(L"3.png")} };
	SetButtonTexture(device, resourceDescriptors.get(), resourceUpload, filenames, &button);

	auto uploadResourcesFinished = resourceUpload.End(deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();
}

TEST(MainLoop, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}