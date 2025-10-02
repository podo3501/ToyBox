#include "pch.h"
#include "UIFixture.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureLoadBinder.h"
#include "Helper.h"

namespace UserInterface
{
	TEST_F(TextureResourceBinderTest, BasicOperations)
	{
		auto resBinder = m_uiModule->GetTexResBinder();
		const wstring& filename = L"UI/SampleTexture/Sample_0.png";
		TextureSourceInfo sourceInfo{ filename, TextureSlice::One, {{10, 10, 64, 64}} };
		EXPECT_EQ(resBinder->GetBindingKey(sourceInfo), "BackImage1");

		vector<TextureSourceInfo> horzAreas = GetAreas(resBinder, filename, TextureSlice::ThreeH);
		EXPECT_TRUE(!horzAreas.empty() && horzAreas[0].sources.size() == 3);
		vector<TextureSourceInfo> vertAreas = GetAreas(resBinder, filename, TextureSlice::ThreeV);
		EXPECT_NE(horzAreas.size(), vertAreas.size());

		TextureSourceInfo testSourceInfo{ L"TestTexFilename.json", TextureSlice::One, {} };
		TextureFontInfo testFontInfo{ L"TestFontFilename.json" };
		EXPECT_TRUE(resBinder->AddFontKey(L"Test", testFontInfo));
		EXPECT_TRUE(resBinder->AddTextureKey("Test", testSourceInfo));
		EXPECT_TRUE(resBinder->RenameFontKey(L"Test", L"NewTest"));
		EXPECT_TRUE(resBinder->RenameTextureKey("Test", "NewTest"));
		EXPECT_TRUE(resBinder->RemoveKeyByFilename(L"TestFontFilename.json"));
		EXPECT_TRUE(resBinder->RemoveKeyByFilename(L"TestTexFilename.json"));

		EXPECT_TRUE(resBinder->GetTextureKeys(TextureSlice::ThreeH).size());

		TestSourceBinderWriteRead(resBinder);

		auto newResBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
		m_mockRenderer->LoadTextureBinder(newResBinder.get());

		EXPECT_EQ(*resBinder, *newResBinder);
	}

	static void TestLoadTextureFromFile(IRenderer* renderer, TextureLoadBinder* loadBinder, const wstring& filename)
	{
		loadBinder->AddTexture(filename);
		renderer->LoadTextureBinder(loadBinder);
		auto sourceInfo = loadBinder->GetSourceInfo(filename);
		EXPECT_TRUE(sourceInfo);
	}

	TEST_F(TextureResourceBinderTest, TextureFileLoading)
	{
		auto loadBinder = make_unique<TextureLoadBinder>();

		const wstring& sampleFilename = L"UI/SampleTexture/Sample_0.png";
		TestLoadTextureFromFile(m_mockRenderer.get(), loadBinder.get(), sampleFilename);

		const wstring& optionFilename = L"UI/SampleTexture/Option.png";
		TestLoadTextureFromFile(m_mockRenderer.get(), loadBinder.get(), optionFilename);
	}
}