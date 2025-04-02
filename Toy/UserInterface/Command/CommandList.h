#pragma once

template<typename CommandType>
class CommandList
{
public:
	~CommandList() = default;
	CommandList() = default;

	bool Undo() noexcept;
	bool Redo() noexcept;

protected:
	template <typename CommandTypeDerived, typename... ParamTypes>
	bool ApplyCommand(ParamTypes&&... params);
	void AddOrMergeCommand(unique_ptr<CommandType> command) noexcept;

private:
	unique_ptr<CommandType> TryMergeCommand(unique_ptr<CommandType> cmd) noexcept;

	vector<unique_ptr<CommandType>> m_commandList;
	int m_index{ -1 };
};

template<typename CommandType>
bool CommandList<CommandType>::Undo() noexcept
{
	if (m_index < 0) return true;

	auto& cmd = m_commandList[m_index];
	auto result = cmd->Undo();

	if (!result)
	{
		assert(false && "Undo�Լ��� �����ϴ� ���� ������ ���̱� ������ �����ؼ��� �ȵȴ�.");
		return result;
	}

	m_index--;
	return true;
}

template<typename CommandType>
bool CommandList<CommandType>::Redo() noexcept
{
	if (m_index + 1 >= static_cast<int>(m_commandList.size())) return true;

	auto& cmd = m_commandList[m_index + 1];
	auto result = cmd->Redo();

	if (!result)
	{
		assert(false && "Redo�Լ��� �����ߴ� ���� �ٽ� �����ϴ� ���̱� ������ �����ؼ��� �ȵȴ�.");
		return result;
	}

	m_index++;
	return true;
}

template<typename CommandType>
unique_ptr<CommandType> CommandList<CommandType>::TryMergeCommand(unique_ptr<CommandType> cmd) noexcept
{
	if (m_commandList.empty()) return cmd;

	auto& preCmd = m_commandList.back();
	return preCmd->Merge(move(cmd));
}

template<typename CommandType>
void CommandList<CommandType>::AddOrMergeCommand(unique_ptr<CommandType> command) noexcept
{
	if (m_index < static_cast<int>(m_commandList.size()) - 1)
		m_commandList.resize(m_index + 1);

	//Merge�� �õ��ϰ� �����ϸ� command�� �ٽ� �����ش�. 
	// �ƴϸ� IsMerge�� �ҷ���� �ϴµ� �ִ��� public �Լ��� �� �����°� ���� �ڵ����̴�.
	command = TryMergeCommand(move(command));
	if (!command) return;

	m_commandList.emplace_back(move(command));
	m_index++;
}

template<typename CommandType>
template <typename CommandTypeDerived, typename... ParamTypes>
bool CommandList<CommandType>::ApplyCommand(ParamTypes&&... params)
{
	unique_ptr<CommandType> command = make_unique<CommandTypeDerived>(forward<ParamTypes>(params)...);
	if (!command->Execute()) return false;

	AddOrMergeCommand(move(command));
	return true;
}