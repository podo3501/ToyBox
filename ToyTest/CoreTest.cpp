#include "pch.h"
#include "CoreTestFixture.h"
#include "../Core/Utility.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
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

	static size_t LoadAndCheckRenderTexture(ITextureController* texController, IComponent* component, const XMUINT2& size, const XMINT2& pos)
	{
		size_t index{ 0 };
		EXPECT_TRUE(texController->CreateRenderTexture(component, size, pos, index, nullptr));
		return index;
	}

	static size_t LoadAndCheckFont(ITextureLoad* load, const wstring& filename)
	{
		size_t fontIdx{ 0 };
		EXPECT_TRUE(load->LoadFont(filename, fontIdx));
		return fontIdx;
	}
}