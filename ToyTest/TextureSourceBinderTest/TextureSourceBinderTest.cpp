#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureLoadBinder.h"
#include "../TestHelper.h"

namespace UserInterfaceTest
{
	TEST_F(TextureResourceBinderTest, BasicOperations)
	{
		const wstring& filename = L"UI/SampleTexture/Sample_0.png";
		EXPECT_TRUE((GetTextureArea(m_resBinder.get(), "BackImage1", 0) == Rectangle{ 10, 10, 64, 64 })); //?!? 테스트 밖에 함수가 사용되지 않고 있다.
		TextureSourceInfo sourceInfo{ filename, ImagePart::One, {{10, 10, 64, 64}} };
		EXPECT_EQ(m_resBinder->GetBindingKey(sourceInfo), "BackImage1");

		vector<TextureSourceInfo> horzAreas = m_resBinder->GetAreas(filename, ImagePart::ThreeH);
		EXPECT_TRUE(!horzAreas.empty() && horzAreas[0].sources.size() == 3);
		vector<TextureSourceInfo> vertAreas = m_resBinder->GetAreas(filename, ImagePart::ThreeV);
		EXPECT_NE(horzAreas.size(), vertAreas.size());

		TextureSourceInfo testSourceInfo{ L"TestTexFilename.json", ImagePart::One, {} };
		TextureFontInfo testFontInfo{ L"TestFontFilename.json" };
		EXPECT_TRUE(m_resBinder->AddFontKey(L"Test", testFontInfo));
		EXPECT_TRUE(m_resBinder->AddTextureKey("Test", testSourceInfo));
		EXPECT_TRUE(m_resBinder->RenameFontKey(L"Test", L"NewTest"));
		EXPECT_TRUE(m_resBinder->RenameTextureKey("Test", "NewTest"));
		EXPECT_TRUE(m_resBinder->RemoveKeyByFilename(L"TestFontFilename.json"));
		EXPECT_TRUE(m_resBinder->RemoveKeyByFilename(L"TestTexFilename.json"));

		EXPECT_TRUE(m_resBinder->GetTextureKeys(ImagePart::ThreeH).size());

		TestSourceBinderWriteRead(m_resBinder);

		auto resBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
		m_renderer->LoadTextureBinder(resBinder.get());

		EXPECT_EQ(*m_resBinder, *resBinder);
	}

	static void TestLoadTextureFromFile(IRenderer* renderer, TextureLoadBinder* loadBinder, const wstring& filename)
	{
		loadBinder->AddTexture(filename);
		renderer->LoadTextureBinder(loadBinder);
		auto sourceInfo = loadBinder->GetSourceInfo(filename);
		EXPECT_TRUE(sourceInfo);
	}

	TEST_F(TextureResourceBinderTest, TextureFileLoadingTest)
	{
		auto loadBinder = make_unique<TextureLoadBinder>();

		const wstring& sampleFilename = L"UI/SampleTexture/Sample_0.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), sampleFilename);

		const wstring& optionFilename = L"UI/SampleTexture/Option.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), optionFilename);
	}
}