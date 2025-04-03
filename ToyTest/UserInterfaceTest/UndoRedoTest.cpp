#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid3.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid9.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/Command/TexResCommandList/TexResCommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"

namespace UserInterfaceTest
{
	TEST_F(UndoRedoTest, TextureResourceBinder)
	{
		TexResCommandList c;
		vector<unique_ptr<TextureResourceBinder>> h;
		const auto& rb = m_resBinder.get();

		CaptureSnapshot(c, h);
		ExecuteAndCapture(c, h, [&] { c.AddFontKey(rb, L"TestFontKey", TextureFontInfo{ L"TestFontKey.spritefont" }); });
		ExecuteAndCapture(c, h, [&] { c.AddTextureKey(rb, "TestTexKey", TextureSourceInfo{ L"TestTexture.png" }); });
		ExecuteAndCapture(c, h, [&] { c.AddTextureKey(rb, "RemoveByFile", TextureSourceInfo{ L"RemoveByFile.png" }); });
		ExecuteAndCapture(c, h, [&] { c.RemoveKeyByFilename(rb, L"RemoveByFile.png"); });
		ExecuteAndCapture(c, h, [&] { c.ModifyTextureSourceInfo(rb, "TestTexKey", TextureSourceInfo{ L"RenameTexture.png" }); });
		ExecuteAndCapture(c, h, [&] { c.RenameFontKey(rb, L"TestFontKey", L"NewTestFontKey"); });
		ExecuteAndCapture(c, h, [&] { c.RenameTextureKey(rb, "TestTexKey", "NewTestTexKey"); });
		ExecuteAndCapture(c, h, [&] { c.RemoveTextureKey(rb, "NewTestTexKey"); });
		ExecuteAndCapture(c, h, [&] { c.RemoveFontKey(rb, L"NewTestFontKey"); });
		VerifyUndoRedo(c, h);
	}

	//////////////////////////////////////////////////////////
	TEST_F(UndoRedoTest, UserInterface)
	{
		UICommandList c;
		vector<unique_ptr<UIComponent>> h;
		UIComponent* panel = m_panel.get();
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::Center }, "BackImage1"));

		CaptureSnapshot(c, h);
		ExecuteAndCapture(c, h, [&] { c.AttachComponent(panel, move(img1), { 111, 222 }); });
		ExecuteAndCapture(c, h, [&] { c.SetRelativePosition(img1Ptr, { 123, 234 }); });
		ExecuteAndCapture(c, h, [&] { c.SetSize(img1Ptr, { 32, 32 }); });
		ExecuteAndCapture(c, h, [&] { c.RenameRegion(img1Ptr, "region"); });
		ExecuteAndCapture(c, h, [&] { c.Rename(img1Ptr, "img1"); });
		ExecuteAndCapture(c, h, [&] { c.DetachComponent(img1Ptr); });
		VerifyUndoRedo(c, h);
	}
}