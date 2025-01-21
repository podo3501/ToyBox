#pragma once
#include "Command.h"

class UIComponent;

class RelativePositionCommand : public Command
{
public:
	RelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept;

	virtual CommandID GetTypeID() const noexcept override { return CommandID::RelativePosition; }
	virtual bool Execute() override;
	virtual bool Redo() override;
	virtual bool Undo() override;
	virtual bool IsMerge() noexcept { return true; }
	virtual void Merge(unique_ptr<Command>) noexcept override;

private:
	UIComponent* m_component;
	XMINT2 m_position{};
	XMINT2 m_beforePosition{};
};

//////////////////////////////////////////////////////////////////

class SizeCommand : public Command
{
public:
	SizeCommand(UIComponent* component, const XMUINT2& size) noexcept;

	virtual CommandID GetTypeID() const noexcept override { return CommandID::Size; }
	virtual bool Execute() override;
	virtual bool Redo() override;
	virtual bool Undo() override;
	virtual bool IsMerge() noexcept { return true; }
	virtual void Merge(unique_ptr<Command>) noexcept override;

private:
	UIComponent* m_component;
	XMUINT2 m_size{};
	XMUINT2 m_beforeSize{};
};

//////////////////////////////////////////////////////////////////

class RenameCommand : public Command
{
public:
	RenameCommand(UIComponent* component, const string& name) noexcept;

	virtual CommandID GetTypeID() const noexcept override { return CommandID::Rename; }
	virtual bool Execute() override;
	virtual bool Redo() override;
	virtual bool Undo() override;
	virtual bool IsMerge() noexcept { return false; }

private:
	UIComponent* m_component;
	string m_name;
	string m_beforeName;
};

//////////////////////////////////////////////////////////////////

class ImageGrid1;
class SourceCommand : public Command
{
public:
	SourceCommand(UIComponent* component, const Rectangle& source) noexcept;

	virtual CommandID GetTypeID() const noexcept override { return CommandID::Source; }
	virtual bool Execute() override;
	virtual bool Redo() override;
	virtual bool Undo() override;
	virtual bool IsMerge() noexcept { return true; }
	virtual void Merge(unique_ptr<Command>) noexcept override;

private:
	ImageGrid1* m_imgGrid1;
	Rectangle m_source{};
	Rectangle m_beforeSource{};
};