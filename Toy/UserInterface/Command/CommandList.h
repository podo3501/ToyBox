#pragma once

class Command;
class UIComponent;

class CommandList
{
public:
	~CommandList();
	CommandList();

	bool RelativePosition(UIComponent* component, const XMINT2 relativePos);
	bool Undo();
	bool Redo();

private:
	vector<unique_ptr<Command>> m_commandList;
	int m_index{ -1 };
};