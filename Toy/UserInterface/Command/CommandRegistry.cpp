#include "pch.h"
#include "CommandRegistry.h"
#include "../../Utility.h"
#include "../UIComponent.h"

RelativePositionCommand::RelativePositionCommand(UIComponent * component, const XMINT2 & relativePos) noexcept :
	m_component{ component }, m_position{ relativePos }
{}

bool RelativePositionCommand::Execute()
{
	ReturnIfFalse(m_component->GetRelativePosition(m_beforePosition));
	ReturnIfFalse(m_component->SetRelativePosition(m_position));

	return true;
}

bool RelativePositionCommand::Undo()
{
	ReturnIfFalse(m_component->SetRelativePosition(m_beforePosition));

	return true;
}

bool RelativePositionCommand::Redo()
{
	ReturnIfFalse(m_component->SetRelativePosition(m_position));

	return true;
}