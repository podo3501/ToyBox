#pragma once
#include "Command.h"

class UIComponent;

class RelativePositionCommand : public Command
{
public:
	RelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept;

	virtual bool Execute() override;
	virtual bool Redo() override;
	virtual bool Undo() override;

private:
	UIComponent* m_component;
	XMINT2 m_position{};
	XMINT2 m_beforePosition{};
};
