#include "pch.h"
#include "UICommandHistoryT_PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

template<typename Action>
void UICommandHistoryT_PatchTextureStd1::VerifyUndoRedo(Action&& action, PatchTextureStd1* component)
{
	PatchTextureStd1* curComponent = component ? component : m_component;

	auto before = Clone(m_main);
	action(m_command.get(), curComponent);

	auto after = Clone(m_main);
	EXPECT_NE(*before, *after);

	m_command->Undo();
	EXPECT_EQ(*m_main, *before);

	m_command->Redo();
	EXPECT_EQ(*m_main, *after);
}

namespace D::UserInterface::CommandHistory
{
	TEST_F(UICommandHistoryT_PatchTextureStd1, AttachComponent)
	{
		auto [owner, component]  = GetPtrs(CreateComponent<PatchTextureStd1>("One32"));

		VerifyUndoRedo([this, o = move(owner)](UICommandHistory* c, auto comp) mutable {
				c->AttachComponent(m_main, move(o)); //코드가 복잡해졌는데 다른거 볼거 없고 이게 쓰는법.
				}, component);
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, ChangeRelativePosition)
	{
		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->ChangeRelativePosition(comp, { 10, 10 });
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, SetSize)
	{
		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->SetSize(comp, { 48, 48 }); //초기값이 32값이니까 32와 다른 값
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, RenameRegion)
	{
		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->RenameRegion(comp, "region");
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, Rename)
	{
		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->Rename(comp, "name");
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, DetachComponent)
	{
		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->DetachComponent(comp);
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, FitToTextureSource)
	{
		ChangeSize(m_component, { 10, 10 });

		VerifyUndoRedo([](UICommandHistory* c, auto comp) {
			c->FitToTextureSource(comp);
			});
	}

	TEST_F(UICommandHistoryT_PatchTextureStd1, ChangeBindKey)
	{
		VerifyUndoRedo([this](UICommandHistory* c, auto comp) {
			c->ChangeBindKey(comp, GetResBinder(), "One48");
			});
	}
}