#pragma once
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"

class TexResCommandHistoryT : public ::testing::Test
{
protected:
	virtual void SetUp() override {};
	virtual void TearDown() override {};

	TexResCommandHistory m_command;
};