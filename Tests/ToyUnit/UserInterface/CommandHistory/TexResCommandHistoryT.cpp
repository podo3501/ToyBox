#include "pch.h"
#include "TexResCommandHistoryT.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"

template <typename Action>
void TexResCommandHistoryT::VerifyUndoRedo(Action&& action)
{
	TextureResourceBinder* binder = m_command->GetReceiver();

	auto before = make_unique<TextureResourceBinder>(*binder);
	action(m_command.get()); 

	auto after = make_unique<TextureResourceBinder>(*binder);
	EXPECT_NE(*before, *after);

	m_command->Undo();
	EXPECT_EQ(*binder, *before);

	m_command->Redo();
	EXPECT_EQ(*binder, *after);
}

namespace D::UserInterface::CommandHistory
{
	TEST_F(TexResCommandHistoryT, AddFontKey)
	{
		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->AddFontKey(L"TestFontKey", TextureFontInfo{ L"TestFontKey.spritefont" }); //이렇게 사용한다.
			});
	}

	TEST_F(TexResCommandHistoryT, AddTextureKey)
	{
		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->AddTextureKey("TestTexKey", TextureSourceInfo{ L"TestTexture.png" });
			});
	}

	TEST_F(TexResCommandHistoryT, ModifyTextureSourceInfo)
	{
		//수정하는 것을 테스트 하기 위해서 데이터를 넣는다.
		m_command->AddTextureKey("TestTexKey", TextureSourceInfo{ L"TestTexture.png" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->ModifyTextureSourceInfo("TestTexKey", TextureSourceInfo{ L"RenameTexture.png" });
			});
	}

	TEST_F(TexResCommandHistoryT, RemoveFontKey)
	{
		m_command->AddFontKey(L"TestFontKey", TextureFontInfo{ L"TestFontKey.spritefont" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->RemoveFontKey(L"TestFontKey");
			});
	}

	TEST_F(TexResCommandHistoryT, RemoveKeyByFilename)
	{
		m_command->AddTextureKey("RemoveByFile", TextureSourceInfo{ L"RemoveByFile.png" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->RemoveKeyByFilename(L"RemoveByFile.png");
			});
	}

	TEST_F(TexResCommandHistoryT, RemoveTextureKey)
	{
		m_command->AddTextureKey("TestTexKey", TextureSourceInfo{ L"TestTexture.png" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->RemoveTextureKey("TestTexKey");
			});
	}

	TEST_F(TexResCommandHistoryT, RenameFontKey)
	{
		m_command->AddFontKey(L"TestFontKey", TextureFontInfo{ L"TestFontKey.spritefont" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->RenameFontKey(L"TestFontKey", L"NewTestFontKey");
			});
	}

	TEST_F(TexResCommandHistoryT, RenameTextureKey)
	{
		m_command->AddTextureKey("TestTexKey", TextureSourceInfo{ L"TestTexture.png" });

		VerifyUndoRedo([](TexResCommandHistory* cmd) {
			cmd->RenameTextureKey("TestTexKey", "NewTestTexKey");
			});
	}
}