#include "pch.h"
#include "CoreTestFixture.h"
#include "../Core/TextureIndexing.h"
#include "../Core/DeviceResources.h"
#include "../Core/Utility.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "TestHelper.h"

namespace BasicCore
{
	TEST(CoreUtility, CycleIterator)
	{
		CycleIterator addIter(1, 7);
		for (int a = 1; a != 7; ++a)
			addIter.Increase();
		EXPECT_EQ(addIter.GetCurrent(), 1);

		CycleIterator subIter(-1, 7);
		for (int a = 1; a != 2; ++a)
			subIter.Decrease();
		EXPECT_EQ(subIter.GetCurrent(), 6);
	}

	//Rectangle 함수인자를 optional로 하면 괜찮을 꺼 같다. ImageGrid1의 m_source도 optional이 돼야 하겠지만.
	static size_t LoadAndCheckTexture(ILoadData* load, const wstring& filename, const Rectangle& rect, XMUINT2* size)
	{
		size_t index{ 0 };
		EXPECT_TRUE(load->LoadTexture(filename, &rect, index, size));
		return index;
	}

	static size_t LoadAndCheckTexture(ILoadData* load, const wstring& filename, Rectangle* rect, XMUINT2* size)
	{
		size_t index{ 0 };
		EXPECT_TRUE(load->LoadTexture(filename, rect, index, size));
		return index;
	}

	static size_t LoadAndCheckRenderTexture(ILoadData* load, const XMUINT2& size, IComponent* component)
	{
		size_t index{ 0 };
		EXPECT_TRUE(load->CreateRenderTexture(size, component, index, nullptr));
		return index;
	}

	static size_t LoadAndCheckFont(ILoadData* load, const wstring& filename)
	{
		size_t fontIdx{ 0 };
		EXPECT_TRUE(load->LoadFont(filename, fontIdx));
		return fontIdx;
	}

	///////////////////////////////////////////////////////////////////////////////////

	static bool TexturLoadingTest(ILoadData* load)
	{
		wstring filename{ L"Resources/UI/SampleTexture/Sample_0.png" };
		XMUINT2 size{};

		EXPECT_EQ(LoadAndCheckTexture(load, filename, { 0, 0, 31, 35 }, &size), 0);
		EXPECT_EQ(LoadAndCheckTexture(load, filename, { 0, 0, 64, 35 }, &size), 1);
		EXPECT_EQ(LoadAndCheckTexture(load, filename, { 31, 35, 2, 35 }, &size), 2);
		EXPECT_EQ(LoadAndCheckTexture(load, filename, { 0, 0, 31, 35 }, &size), 0);
		EXPECT_EQ(LoadAndCheckTexture(load, filename, nullptr, &size), 3);

		//auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });
		//EXPECT_EQ(LoadAndCheckRenderTexture(load, img1->GetSize(), img1.get()), 3);

		return true;
	}

	static bool LoadFont(ILoadData* load)
	{
		wstring hangleFilename{ L"Resources/UI/Font/HangleS16.spritefont" };
		wstring englishFilename{ L"Resources/UI/Font/CourierNewBoldS18.spritefont" };

		EXPECT_EQ(LoadAndCheckFont(load, hangleFilename), 0);
		EXPECT_EQ(LoadAndCheckFont(load, englishFilename), 1);

		return true;
	}
	
	TEST_F(IRendererTest, LoadingTest)
	{
		unique_ptr<TestComponent> testComponent = make_unique<TestComponent>();
		testComponent->SetLoadTestFunction(TexturLoadingTest);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));

		testComponent->SetLoadTestFunction(LoadFont);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));
	}
}