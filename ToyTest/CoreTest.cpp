#include "pch.h"
#include "CoreTestFixture.h"
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

	static pair<size_t, XMUINT2> LoadAndCheckTexture(ITextureLoad* load, const wstring& filename)
	{
		size_t index{ 0 };
		XMUINT2 size{};
		EXPECT_TRUE(load->LoadTexture(filename, index, &size, nullptr));
		return { index, size };
	}

	static size_t LoadAndCheckRenderTexture(ITextureController* texController, const XMUINT2& size, IComponent* component)
	{
		size_t index{ 0 };
		EXPECT_TRUE(texController->CreateRenderTexture(size, component, index, nullptr));
		return index;
	}

	static size_t LoadAndCheckFont(ITextureLoad* load, const wstring& filename)
	{
		size_t fontIdx{ 0 };
		EXPECT_TRUE(load->LoadFont(filename, fontIdx));
		return fontIdx;
	}

	///////////////////////////////////////////////////////////////////////////////////

	static bool TexturLoadingTest(ITextureLoad* load)
	{
		wstring sample{ L"Resources/UI/SampleTexture/Sample_0.png" };
		wstring option{ L"Resources/UI/Texture/Option.png" };

		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(0, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(0, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, option), Pair(1, XMUINT2{ 512, 512 }));

		return true;
	}

	static bool LoadFont(ITextureLoad* load)
	{
		wstring hangleFilename{ L"Resources/UI/Font/HangleS16.spritefont" };
		wstring englishFilename{ L"Resources/UI/Font/CourierNewBoldS18.spritefont" };

		EXPECT_EQ(LoadAndCheckFont(load, hangleFilename), 10);
		EXPECT_EQ(LoadAndCheckFont(load, hangleFilename), 10);
		EXPECT_EQ(LoadAndCheckFont(load, englishFilename), 11);

		return true;
	}

	static bool ReloadingTest(ITextureLoad* load)
	{
		wstring sample{ L"Resources/UI/SampleTexture/Sample_0.png" };
		wstring option{ L"Resources/UI/Texture/Option.png" };

		EXPECT_THAT(LoadAndCheckTexture(load, sample), Pair(1, XMUINT2{ 512, 512 }));
		EXPECT_THAT(LoadAndCheckTexture(load, option), Pair(0, XMUINT2{ 512, 512 }));

		return true;
	}
	
	TEST_F(IRendererTest, LoadingTest)
	{
		unique_ptr<TestComponent> testComponent = make_unique<TestComponent>();
		testComponent->SetLoadTestFunction(TexturLoadingTest);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));

		auto texController = m_renderer->GetTextureController();
		auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });
		for(auto idx : views::iota(2, 10))
			EXPECT_EQ(LoadAndCheckRenderTexture(texController, img1->GetSize(), img1.get()), idx);

		testComponent->SetLoadTestFunction(LoadFont);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));

		texController->ReleaseTexture(0);
		texController->ReleaseTexture(0); //2번 로딩하면 2번 지워줘야 한다.
		texController->ReleaseTexture(1);

		testComponent->SetLoadTestFunction(ReloadingTest);
		EXPECT_TRUE(m_renderer->LoadComponent(testComponent.get()));
	}
}