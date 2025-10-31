#include "pch.h"
#include "UICommandHistoryT_PatchTextureStd1.h"

template<typename Action>
void UICommandHistoryT_PatchTextureStd1::VerifyUndoRedo(Action&& action, PatchTextureStd1* component)
{
	PatchTextureStd1* curComponent = component ? component : m_component;

	auto before = m_main->Clone();
	action(m_command.get(), curComponent);

	auto after = m_main->Clone();
	EXPECT_NE(*before, *after);

	m_command->Undo();
	EXPECT_EQ(*m_main, *before);

	m_command->Redo();
	EXPECT_EQ(*m_main, *after);
}

namespace UserInterfaceT::CommandHistoryT
{
	TEST_F(UICommandHistoryT_PatchTextureStd1, AttachComponent)
	{
		auto [owner, component]  = GetPtrs(CreateComponent<PatchTextureStd1>("One32"));

		VerifyUndoRedo([this, o = move(owner)](UICommandHistory* c, auto comp) mutable {
				c->AttachComponent(m_main, move(o)); //�ڵ尡 ���������µ� �ٸ��� ���� ���� �̰� ���¹�.
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
			c->SetSize(comp, { 48, 48 }); //�ʱⰪ�� 32���̴ϱ� 32�� �ٸ� ��
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
		m_component->ChangeSize({ 10, 10 });

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