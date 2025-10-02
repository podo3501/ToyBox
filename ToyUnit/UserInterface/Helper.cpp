#include "pch.h"
#include "Helper.h"
#include "IComponent.h"
#include "Toy/GameConfig.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/JsonOperation/JsonOperation.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Shared/Utils/GeometryExt.h"

//?!?이 함수는 사라질 예정
unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename)
{
	JsonOperation::WriteJsonToFile(write, filename);
	
	unique_ptr<UIComponent> read;
	JsonOperation::ReadJsonFromFile(filename, read);
	
	EXPECT_TRUE(*write == *read);

	return move(read);
}

unique_ptr<UIModule> WriteReadTest(IRenderer* renderer, unique_ptr<UIModule>& write, const wstring& filename)
{
	if (!write->Write(filename)) return nullptr;
	auto resBinder = write->GetTexResBinder();
	unique_ptr<UIModule> read = CreateUIModule(filename, renderer, resBinder->GetJsonFilename());

	EXPECT_TRUE(*write == *read);
	return move(read);
}

unique_ptr<UIComponent> WriteReadTest(TextureResourceBinder* binder,
	unique_ptr<UIComponent>& write, const wstring& filename)
{
	JsonOperation::WriteJsonToFile(write, filename);

	unique_ptr<UIComponent> read;
	JsonOperation::ReadJsonFromFile(filename, read);
	EXPECT_TRUE(read->BindTextureSourceInfo(binder, nullptr));
	EXPECT_TRUE(*write == *read);

	return move(read);
}

bool TestSourceBinderWriteRead(TextureResourceBinder* write, const wstring& filename)
{
	ReturnIfFalse(write->Save(filename));

	unique_ptr<TextureResourceBinder> read = make_unique<TextureResourceBinder>();
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
	size_t expIndex, const vector<RECT>& expDests, const vector<RECT>& expSrcs) noexcept
{
	EXPECT_TRUE(index == expIndex);
	EXPECT_EQ(expDests.size(), expSrcs.size());
	bool result = false;
	for(size_t idx : views::iota(0u, expDests.size())) //views::zip이 있으면 간단하게 되는데 c++23부터 지원
		result |= IsTrue(dest, expDests[idx], *source, expSrcs[idx]);
	EXPECT_TRUE(result);
}

vector<RECT> GetSources(TextureResourceBinder* resBinder, const string& key) noexcept
{
	const auto& rectangles = GetSourcesOfBindKey(resBinder, key);
	if (!rectangles) return {};

	return RectanglesToRects(rectangles->get());
}