#pragma once

class Window;
struct ILoadData;

namespace DX { class DeviceResources; }

class CoreTest : public ::testing::Test
{
public:
	CoreTest();
	~CoreTest();

protected:
	void SetUp() override;
	void TearDown() override;

	bool LoadResources(function<bool(ILoadData*)> funcLoad, ILoadData* load);

	unique_ptr<Window> m_window;
	unique_ptr<DX::DeviceResources> m_deviceResources;
	unique_ptr<GraphicsMemory> m_graphicsMemory;
	unique_ptr<DescriptorHeap> m_resourceDescriptors;
	unique_ptr<ResourceUploadBatch> m_batch;
	unique_ptr<SpriteBatch> m_spriteBatch;

private:
	bool Initialize();
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
};