#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureLoadBinder.h"
#include "../TestHelper.h"

namespace UserInterfaceTest
{
	TEST_F(TextureSourceBinderTest, BasicOperations)
	{
		const wstring& filename = L"UI/SampleTexture/Sample_0.png";
		EXPECT_TRUE((GetTextureArea(m_sourceBinder.get(), "BackImage1", 0) == Rectangle{ 10, 10, 64, 64 })); //?!? �׽�Ʈ �ۿ� �Լ��� ������ �ʰ� �ִ�.
		TextureSourceInfo sourceInfo{ filename, ImagePart::One, {{10, 10, 64, 64}} };
		EXPECT_EQ(m_sourceBinder->GetBindingKey(sourceInfo), "BackImage1");

		vector<TextureSourceInfo> horzAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeH);
		EXPECT_TRUE(!horzAreas.empty() && horzAreas[0].sources.size() == 3);
		vector<TextureSourceInfo> vertAreas = m_sourceBinder->GetAreas(filename, ImagePart::ThreeV);
		EXPECT_NE(horzAreas.size(), vertAreas.size());

		TextureSourceInfo testSourceInfo{ L"TestTexFilename.json", ImagePart::One, {} };
		TextureFontInfo testFontInfo{ L"TestFontFilename.json" };
		EXPECT_TRUE(m_sourceBinder->AddFontKey(L"Test", testFontInfo));
		EXPECT_TRUE(m_sourceBinder->AddTextureKey("Test", testSourceInfo));
		EXPECT_TRUE(m_sourceBinder->RenameFontKey(L"Test", L"NewTest"));
		EXPECT_TRUE(m_sourceBinder->RenameTextureKey("Test", "NewTest"));
		EXPECT_TRUE(m_sourceBinder->RemoveKeyByFilename(L"TestFontFilename.json"));
		EXPECT_TRUE(m_sourceBinder->RemoveKeyByFilename(L"TestTexFilename.json"));

		EXPECT_TRUE(m_sourceBinder->GetTextureKeys(ImagePart::ThreeH).size());

		TestSourceBinderWriteRead(m_sourceBinder);

		auto sourceBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
		m_renderer->LoadTextureBinder(sourceBinder.get());

		EXPECT_EQ(*m_sourceBinder, *sourceBinder);
	}

	static void TestLoadTextureFromFile(IRenderer* renderer, TextureLoadBinder* loadBinder, const wstring& filename)
	{
		loadBinder->AddTexture(filename);
		renderer->LoadTextureBinder(loadBinder);
		auto sourceInfo = loadBinder->GetSourceInfo(filename);
		EXPECT_TRUE(sourceInfo);
	}

	TEST_F(TextureSourceBinderTest, TextureFileLoadingTest)
	{
		auto loadBinder = make_unique<TextureLoadBinder>();

		const wstring& sampleFilename = L"UI/SampleTexture/Sample_0.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), sampleFilename);

		const wstring& optionFilename = L"UI/SampleTexture/Option.png";
		TestLoadTextureFromFile(m_renderer.get(), loadBinder.get(), optionFilename);
	}
}