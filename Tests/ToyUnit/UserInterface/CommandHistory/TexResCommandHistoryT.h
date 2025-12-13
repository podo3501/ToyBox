#pragma once
#include "../TextureResourceBinder/MockTextureResourceBinder.h"
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"

class TexResCommandHistoryT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	template <typename Action>
	void VerifyUndoRedo(Action&& action);

	unique_ptr<TexResCommandHistory> m_command;
	unique_ptr<MockTextureResourceBinder> m_resBinder;
};

void TexResCommandHistoryT::SetUp()
{
	m_resBinder = make_unique<MockTextureResourceBinder>();
	m_command = make_unique<TexResCommandHistory>(m_resBinder.get());
}