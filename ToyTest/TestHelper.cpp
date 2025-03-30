#include "pch.h"
#include "TestHelper.h"
#include "../Include/IComponent.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/Utility.h"

unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename)
{
	JsonFile::WriteComponent(write, filename);

	unique_ptr<UIComponent> read;
	JsonFile::ReadComponent(filename, read);

	EXPECT_TRUE(*write == *read);

	return move(read);
}

bool TestSourceBinderWriteRead(unique_ptr<TextureSourceBinder>& write, const wstring& filename)
{
	ReturnIfFalse(write->Save(filename));

	unique_ptr<TextureSourceBinder> read = make_unique<TextureSourceBinder>();
	ReturnIfFalse(read->Load(filename));

	EXPECT_TRUE(*write == *read);

	return true;
}

unique_ptr<UIComponent> TestComponent::CreateClone() const { return nullptr; }
void TestComponent::SetLoadTestFunction(function<bool(ITextureLoad*)> func) { m_loadTestFunc = move(func); }

static bool IsTrue(const RECT& dest, const RECT& destRect, const RECT& source, const RECT& sourceRect) noexcept
{
	if (dest == destRect) { if (source == sourceRect) return true; }
	return false;
}

void TestCoordinates(size_t index, const RECT& dest, const RECT* source,
	const vector<RECT>& expDests, const vector<RECT>& expSrcs) noexcept
{
	EXPECT_TRUE(index == 2); //0, 1은 폰트이다. 
	EXPECT_EQ(expDests.size(), expSrcs.size());
	bool result = false;
	for(size_t idx : views::iota(0u, expDests.size())) //views::zip이 있으면 간단하게 되는데 c++23부터 지원
		result |= IsTrue(dest, expDests[idx], *source, expSrcs[idx]);
	EXPECT_TRUE(result);
}

vector<RECT> GetSources(TextureSourceBinder* sourceBinder, const string& key) noexcept
{
	const auto& rectangles = GetRectangles(sourceBinder, key);
	if (!rectangles) return {};

	return RectanglesToRects(rectangles->get());
}