#pragma once
#include "../Command.h"
#include "../CommandType.h"
#include "../../UIComponent/UIType.h"

class UIComponent;
class AttachComponentCommand : public UICommand
{
public:
	AttachComponentCommand(UIComponent* parent, unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

	unique_ptr<UIComponent> GetFailureResult() noexcept;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::AttachComponent; }
	virtual bool IsMerge(UICommand*) noexcept { return false; }

private:
	UIComponent* m_parent;
	unique_ptr<UIComponent> m_attach;
	XMINT2 m_pos{};
	UIComponent* m_detach;
	unique_ptr<UIComponent> m_failureResult;
};

class DetachComponentCommand : public UICommand
{
public:
	DetachComponentCommand(UIComponent* detach) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

	pair<unique_ptr<UIComponent>, UIComponent*> GetResult() noexcept;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::DetachComponent; }
	virtual bool IsMerge(UICommand*) noexcept { return false; }

private:
	UIComponent* m_detach;
	XMINT2 m_position{};
	unique_ptr<UIComponent> m_component;
	UIComponent* m_parent;
	pair<unique_ptr<UIComponent>, UIComponent*> m_result;
};

class SetRelativePositionCommand : public UICommand
{
public:
	SetRelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::SetRelativePosition; }
	virtual void PostMerge(unique_ptr<UICommand> other) noexcept override;

private:
	CommandRecord<XMINT2> m_record;
};

class SetSizeCommand : public UICommand
{
public:
	SetSizeCommand(UIComponent* component, const XMUINT2& size) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::SetSize; }
	virtual void PostMerge(unique_ptr<UICommand> other) noexcept override;

private:
	CommandRecord<XMUINT2> m_record;
};

class RenameRegionCommand : public UICommand
{
public:
	RenameRegionCommand(UIComponent* component, const string& region) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::SetRegion; }
	virtual bool IsMerge(UICommand*) noexcept { return false; }

private:
	CommandRecord<string> m_record;
};

class RenameCommand : public UICommand
{
public:
	RenameCommand(UIComponent* component, const string& name) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::Rename; }
	virtual bool IsMerge(UICommand*) noexcept { return false; }

private:
	CommandRecord<string> m_record;
};

class TextArea;
class SetTextCommand : public UICommand
{
public:
	SetTextCommand(TextArea* textArea, const wstring& text) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual UICommandID GetTypeID() const noexcept override { return UICommandID::SetText; }
	virtual bool IsMerge(UICommand*) noexcept { return false; }

private:
	CommandRecord<wstring> m_record;
	TextArea* m_textArea;
};