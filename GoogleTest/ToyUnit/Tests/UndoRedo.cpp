#include "pch.h"
#include "Fixtures/FixtureSuite.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"
#include "Toy/UserInterface/UIModule.h"

namespace UserInterface
{
	TEST_F(UndoRedo, TextureResourceBinder)
	{
		vector<unique_ptr<TextureResourceBinder>> h;
		TexResCommandHistory c(GetResBinder());

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
	TEST_F(UndoRedo, UI_PatchTextureStd1)
	{
		UICommandHistory c;
		vector<unique_ptr<UIComponent>> h;
		auto [tex1, tex1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::Center }, "BackImage1"));

		CaptureSnapshot(h);
		ExecuteAndCapture(h, [&] { c.AttachComponent(m_main, move(tex1), {111, 222}); });
		ExecuteAndCapture(h, [&] { c.ChangeRelativePosition(tex1Ptr, { 123, 234 }); });
		ExecuteAndCapture(h, [&] { c.SetSize(tex1Ptr, { 32, 32 }); });
		ExecuteAndCapture(h, [&] { c.RenameRegion(tex1Ptr, "region"); }); //������ region���� ���ƿ��鼭 PatchTextureStd1�� �̸��� ��� ���׾�� �Ѵ�.
		ExecuteAndCapture(h, [&] { c.Rename(tex1Ptr, "tex1"); });
		ExecuteAndCapture(h, [&] { c.DetachComponent(tex1Ptr); });
		VerifyUndoRedo(c, h);
	}

	//////////////////////////////////////////////////////////
	TEST_F(UndoRedo, UI_PatchTextureStd9)
	{
		UICommandHistory c;
		vector<unique_ptr<UIComponent>> h;
		auto [tex9, tex9Ptr] = GetPtrs(CreateComponent<PatchTextureStd9>(UILayout{ {100, 100}, Origin::Center }, "BackImage9"));
		UIEx(m_main).AttachComponent(move(tex9), { 250, 250 });
		m_uiModule->BindTextureResources();

		CaptureSnapshot(h);
		ExecuteAndCapture(h, [&] { c.FitToTextureSource(tex9Ptr); });
		ExecuteAndCapture(h, [&] { c.ChangeBindKey(tex9Ptr, GetResBinder(), "ListBackground9_Normal"); });
		VerifyUndoRedo(c, h);
	}

	//////////////////////////////////////////////////////////
	TEST_F(UndoRedo, UI_TextureSwitcher)
	{
		UICommandHistory c;
		vector<unique_ptr<UIComponent>> h;
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(UILayout{ {100, 100}, Origin::Center },
			TextureSlice::Nine, GetStateKeyMap("ListBackground9"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 250, 250 });
		m_uiModule->BindTextureResources();

		CaptureSnapshot(h);
		ExecuteAndCapture(h, [&] { c.FitToTextureSource(switcherPtr); });
		ExecuteAndCapture(h, [&] { c.ChangeBindKey(switcherPtr, GetResBinder(), "BackImage9"); });
		VerifyUndoRedo(c, h);
	}
}