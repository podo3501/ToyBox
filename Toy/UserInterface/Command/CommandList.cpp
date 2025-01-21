#include "pch.h"
#include "CommandList.h"
#include "Command.h"
#include "CommandRegistry.h"
#include "../../Utility.h"

CommandList::~CommandList() = default;
CommandList::CommandList() = default;

bool CommandList::Undo() noexcept
{
	if (m_index < 0) return true;

	auto& cmd = m_commandList[m_index];
	ReturnIfFalse(cmd->Undo());

	m_index--;
	return true;
}

bool CommandList::Redo() noexcept
{
	if (m_index + 1 >= static_cast<int>(m_commandList.size())) return true;

	auto& cmd = m_commandList[m_index + 1];
	ReturnIfFalse(cmd->Redo());

	m_index++;
	return true;
}

bool CommandList::TryMergeCommand(unique_ptr<Command>& cmd) noexcept
{
	if (!cmd->IsMerge()) return false;
	if (m_commandList.empty()) return false;

	auto& preCmd = m_commandList.back();
	if (preCmd->GetTypeID() != cmd->GetTypeID()) return false;
	
	preCmd->Merge(move(cmd));
	return true;
}

template <typename CommandType, typename... ParamTypes>
bool CommandList::ApplyCommand(UIComponent* component, ParamTypes&&... params)
{
	unique_ptr<Command> command = make_unique<CommandType>(component, forward<ParamTypes>(params)...);
	ReturnIfFalse(command->Execute());

	if (m_index < static_cast<int>(m_commandList.size()) - 1)
		m_commandList.resize(m_index + 1);

	if (TryMergeCommand(command)) return true;

	m_commandList.emplace_back(move(command));
	m_index++;

	return true;
}

bool CommandList::RelativePosition(UIComponent* component, const XMINT2& relativePos)
{
	return ApplyCommand<RelativePositionCommand>(component, relativePos);
}

bool CommandList::Size(UIComponent* component, const XMUINT2& size)
{
	return ApplyCommand<SizeCommand>(component, size);
}

bool CommandList::Rename(UIComponent* component, const string& name)
{
	return ApplyCommand<RenameCommand>(component, name);
}

bool CommandList::Source(UIComponent* imgGrid1, const Rectangle& source)
{
	return ApplyCommand<SourceCommand>(imgGrid1, source);
}
