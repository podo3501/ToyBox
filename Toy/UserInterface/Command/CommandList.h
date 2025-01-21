#pragma once

class Command;
class UIComponent;

class CommandList
{
public:
	~CommandList();
	CommandList();

	bool Undo() noexcept;
	bool Redo() noexcept;

	bool RelativePosition(UIComponent* component, const XMINT2& relativePos);
	bool Size(UIComponent* component, const XMUINT2& size);
	bool Rename(UIComponent* component, const string& name);
	bool Source(UIComponent* imgGrid1, const Rectangle& source);

private:
	bool TryMergeCommand(unique_ptr<Command>& cmd) noexcept;
	template <typename CommandType, typename... ParamTypes>
	bool ApplyCommand(UIComponent* component, ParamTypes&&... params);

	vector<unique_ptr<Command>> m_commandList;
	int m_index{ -1 };
};