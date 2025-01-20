#include "pch.h"
#include "CommandList.h"
#include "Command.h"
#include "CommandRegistry.h"
#include "../../Utility.h"

CommandList::~CommandList() = default;
CommandList::CommandList() = default;

bool CommandList::RelativePosition(UIComponent* component, const XMINT2 relativePos)
{
	if (!m_commandList.empty())
	{
		auto lastCmd = dynamic_cast<RelativePositionCommand*>(m_commandList.back().get());
		int a = 1;
	}
	unique_ptr<Command> cmd = make_unique<RelativePositionCommand>(component, relativePos);
	ReturnIfFalse(cmd->Execute());

	if (m_index < static_cast<int>(m_commandList.size()) - 1)
		m_commandList.resize(m_index + 1);

	m_commandList.emplace_back(move(cmd));
	m_index++;

	return true;
}

bool CommandList::Undo()
{
	if (m_index < 0) return true;

	auto& cmd = m_commandList[m_index];
	ReturnIfFalse(cmd->Undo());

	m_index--;
	return true;
}

bool CommandList::Redo()
{
	if (m_index + 1 >= static_cast<int>(m_commandList.size())) return true;

	auto& cmd = m_commandList[m_index + 1];
	ReturnIfFalse(cmd->Redo());

	m_index++;
	return true;
}