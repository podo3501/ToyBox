#pragma once
#include "UserInterface/BaseModuleT.h"
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"

class TexResCommandHistoryT : public BaseModuleT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	template <typename Action>
	void VerifyUndoRedo(Action&& action);

	unique_ptr<TexResCommandHistory> m_command;
};

void TexResCommandHistoryT::SetUp()
{
	BaseModuleT::SetUp();

	m_command = make_unique<TexResCommandHistory>(m_uiModule->GetTexResBinder());
}