#include "pch.h"
#include "CommandRegistry.h"
#include "../../Utility.h"
#include "../UIComponent.h"
#include "../Component/ImageGrid1.h"

RelativePositionCommand::RelativePositionCommand(UIComponent * component, const XMINT2 & relativePos) noexcept :
	m_component{ component }, m_position{ relativePos }
{}

bool RelativePositionCommand::Execute()
{
	auto beforePos = m_component->GetRelativePosition();
	ReturnIfFalse(beforePos.has_value());
	m_beforePosition = *beforePos;
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

void RelativePositionCommand::Merge(unique_ptr<Command> command) noexcept
{
	auto curCommnad = static_cast<RelativePositionCommand*>(command.get());
	m_position = curCommnad->m_position;
}

//////////////////////////////////////////////////////////////////

SizeCommand::SizeCommand(UIComponent* component, const XMUINT2& size) noexcept :
	m_component{ component }, m_size{ size }
{}

bool SizeCommand::Execute()
{
	m_beforeSize = m_component->GetSize();
	m_component->ChangeSize(m_size);
	return true;
}

bool SizeCommand::Undo() { m_component->ChangeSize(m_beforeSize); return true; }
bool SizeCommand::Redo() { m_component->ChangeSize(m_size);	return true; }

void SizeCommand::Merge(unique_ptr<Command> command) noexcept
{
	auto curCommnad = static_cast<SizeCommand*>(command.get());
	m_size = curCommnad->m_size;
}

//////////////////////////////////////////////////////////////////

RenameCommand::RenameCommand(UIComponent* component, const string& name) noexcept :
	m_component{ component }, m_name{ name }
{}

bool RenameCommand::Execute()
{
	m_beforeName = m_component->GetName();
	return m_component->Rename(m_name);
}

bool RenameCommand::Undo() { return m_component->Rename(m_beforeName); }
bool RenameCommand::Redo() { return m_component->Rename(m_name); }

//////////////////////////////////////////////////////////////////

SourceCommand::SourceCommand(UIComponent* component, const Rectangle& source) noexcept :
	m_imgGrid1{ ComponentCast<ImageGrid1*>(component) }, m_source{ source }
{}

bool SourceCommand::Execute()
{
	m_beforeSource = m_imgGrid1->GetSource();
	m_imgGrid1->SetSource(m_source);
	return true;
}

bool SourceCommand::Undo() { m_imgGrid1->SetSource(m_beforeSource); return true; }
bool SourceCommand::Redo() { m_imgGrid1->SetSource(m_source); return true; }

void SourceCommand::Merge(unique_ptr<Command> command) noexcept
{
	auto curCommnad = static_cast<SourceCommand*>(command.get());
	m_source = curCommnad->m_source;
}