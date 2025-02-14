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

using ::testing::Pair;

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

	static pair<size_t, XMUINT2> LoadAndCheckTexture(ILoadData* load, const wstring& filename)
	{
		size_t index{ 0 };
		XMUINT2 size{};
		EXPECT_TRUE(load->LoadTexture(filename, index, &size));
		return { index, size };
	}

	static size_t LoadAndCheckRenderTexture(IGetValue* value, const XMUINT2& size, IComponent* component)
	{
		size_t index{ 0 };
		EXPECT_TRUE(value->CreateRenderTexture(size, component, index, nullptr));
		return index;
	}

	static size_t LoadAndCheckFont(ILoadData* load, const wstring& filename)
	{
		size_t fontIdx{ 0 };
		EXPECT_TRUE(load->LoadFont(filename, fontIdx));
		return fontIdx;
	}

	///////////////////////////////////////////////////////////////////////////////////

	static void TexturLoadingTest(ILoadData* load)
	{
		wstring sample{ L"Resources/UI/SampleTexture/Sample_0.png" };
		wstring option{ L"Resources/UI/Texture/Option.png" };

		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(0, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(0, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, option), Pair(1, XMUINT2{ 512, 512 }));
	}

	static void LoadFont(ILoadData* load)
	{
		wstring hangleFilename{ L"Resources/UI/Font/HangleS16.spritefont" };
		wstring englishFilename{ L"Resources/UI/Font/CourierNewBoldS18.spritefont" };

		EXPECT_EQ(LoadAndCheckFont(load, hangleFilename), 3);
		EXPECT_EQ(LoadAndCheckFont(load, hangleFilename), 3);
		EXPECT_EQ(LoadAndCheckFont(load, englishFilename), 4);
	}

	static void ReloadingTest(ILoadData* load)
	{
		wstring sample{ L"Resources/UI/SampleTexture/Sample_0.png" };
		wstring option{ L"Resources/UI/Texture/Option.png" };

		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(1, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, option), Pair(0, XMUINT2{ 512, 512 }));
	}
	
	TEST_F(IRendererTest, LoadingTest)
	{
		unique_ptr<TestComponent> testComponent = make_unique<TestComponent>();
		testComponent->SetLoadTestFunction(TexturLoadingTest);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));

		auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });
		EXPECT_EQ(LoadAndCheckRenderTexture(m_renderer->GetValue(), img1->GetSize(), img1.get()), 2);

		testComponent->SetLoadTestFunction(LoadFont);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));

		auto curValue = m_renderer->GetValue();
		curValue->ReleaseTexture(0);
		curValue->ReleaseTexture(1);

		testComponent->SetLoadTestFunction(ReloadingTest);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));
	}
}