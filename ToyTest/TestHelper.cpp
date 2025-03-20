#include "pch.h"
#include "TestHelper.h"
#include "../Include/IComponent.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/Utility.h"

using json = nlohmann::json;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename)
{
	JsonFile::WriteComponent(write, L"Test/Data/JOPTest.json");

	unique_ptr<UIComponent> read;
	JsonFile::ReadComponent(L"Test/Data/JOPTest.json", read);

	EXPECT_TRUE(*write == *read);

	return true;
}

unique_ptr<UIComponent> TestComponent::CreateClone() const { return nullptr; }
bool TestComponent::ImplementLoadResource(ITextureLoad* load) { return m_loadTestFunc(load); };
void TestComponent::SetLoadTestFunction(function<bool(ITextureLoad*)> func) { m_loadTestFunc = move(func); }

static bool IsTrue(const RECT& dest, const RECT& destRect, const RECT& source, const RECT& sourceRect) noexcept
{
	if (dest == destRect) { if (source == sourceRect) return true; }
	return false;
}

void TestRender(size_t index, const RECT& dest, const RECT* source, vector<pair<RECT, RECT>> testCases) noexcept
{
	EXPECT_TRUE(index == 0);
	EXPECT_TRUE(ranges::any_of(testCases, [&](const auto& pair) {
		return IsTrue(dest, pair.first, *source, pair.second);
		}));
}

RECT Binder(TextureSourceBinder* sourceBinder, const string& key, int index) noexcept
{
	return RectangleToRect(sourceBinder->GetArea(key, index));//?!? 테스트 밖에 함수가 사용되지 않고 있다.
}