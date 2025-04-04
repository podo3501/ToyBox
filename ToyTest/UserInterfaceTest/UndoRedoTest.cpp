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
		vector<unique_ptr<TextureResourceBinder>> h;
		TexResCommandList c(m_resBinder.get());

		CaptureSnapshot(h);
		ExecuteAndCapture(h, [&] { c.AddFontKey(L"TestFontKey", TextureFontInfo{ L"TestFontKey.spritefont" }); });
		ExecuteAndCapture(h, [&] { c.AddTextureKey("TestTexKey", TextureSourceInfo{ L"TestTexture.png" }); });
		ExecuteAndCapture(h, [&] { c.AddTextureKey("RemoveByFile", TextureSourceInfo{ L"RemoveByFile.png" }); });
		ExecuteAndCapture(h, [&] { c.RemoveKeyByFilename(L"RemoveByFile.png"); });
		ExecuteAndCapture(h, [&] { c.ModifyTextureSourceInfo("TestTexKey", TextureSourceInfo{ L"RenameTexture.png" }); });
		ExecuteAndCapture(h, [&] { c.RenameFontKey(L"TestFontKey", L"NewTestFontKey"); });
		ExecuteAndCapture(h, [&] { c.RenameTextureKey("TestTexKey", "NewTestTexKey"); });
		ExecuteAndCapture(h, [&] { c.RemoveTextureKey("NewTestTexKey"); });
		ExecuteAndCapture(h, [&] { c.RemoveFontKey(L"NewTestFontKey"); });
		VerifyUndoRedo(c, h);
	}

	//////////////////////////////////////////////////////////
	TEST_F(UndoRedoTest, UserInterface)
	{
		UICommandList c;
		vector<unique_ptr<UIComponent>> h;
		UIComponent* panel = m_panel.get();
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::Center }, "BackImage1"));

		CaptureSnapshot(h);
		ExecuteAndCapture(h, [&] { c.AttachComponent(panel, move(img1), { 111, 222 }); });
		ExecuteAndCapture(h, [&] { c.SetRelativePosition(img1Ptr, { 123, 234 }); });
		ExecuteAndCapture(h, [&] { c.SetSize(img1Ptr, { 32, 32 }); });
		ExecuteAndCapture(h, [&] { c.RenameRegion(img1Ptr, "region"); });
		ExecuteAndCapture(h, [&] { c.Rename(img1Ptr, "img1"); });
		ExecuteAndCapture(h, [&] { c.DetachComponent(img1Ptr); });
		VerifyUndoRedo(c, h);
	}
}