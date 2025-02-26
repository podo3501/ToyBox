#pragma once
#include "Command.h"
#include "CommandType.h"
#include "../UIType.h"

class UIComponent;

template <typename T>
class CommandRecord
{
public:
	CommandRecord(const T& curr) noexcept : current{ curr } {}
	CommandRecord(T&& curr) noexcept : current(move(curr)) {}

	T current{};
	T previous{};
};

class AttachComponentCommand : public Command
{
public:
	AttachComponentCommand(UIComponent* parent, unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

	unique_ptr<UIComponent> GetFailureResult() noexcept;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::AttachComponent; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	UIComponent* m_parent;
	unique_ptr<UIComponent> m_attach;
	XMINT2 m_pos{};
	UIComponent* m_detach;
	unique_ptr<UIComponent> m_failureResult;
};

class DetachComponentCommand : public Command
{
public:
	DetachComponentCommand(UIComponent* detach) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

	pair<unique_ptr<UIComponent>, UIComponent*> GetResult() noexcept;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::DetachComponent; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	UIComponent* m_detach;
	XMINT2 m_position{};
	unique_ptr<UIComponent> m_component;
	UIComponent* m_parent;
	pair<unique_ptr<UIComponent>, UIComponent*> m_result;
};

class SetRelativePositionCommand : public Command
{
public:
	SetRelativePositionCommand(UIComponent* component, const XMINT2& relativePos) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetRelativePosition; }
	virtual void PostMerge(unique_ptr<Command> other) noexcept override;

private:
	CommandRecord<XMINT2> m_record;
};

class SetSizeCommand : public Command
{
public:
	SetSizeCommand(UIComponent* component, const XMUINT2& size) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetSize; }
	virtual void PostMerge(unique_ptr<Command> other) noexcept override;

private:
	CommandRecord<XMUINT2> m_record;
};

class RenameRegionCommand : public Command
{
public:
	RenameRegionCommand(UIComponent* component, const string& region) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetRegion; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	CommandRecord<string> m_record;
};

class RenameCommand : public Command
{
public:
	RenameCommand(UIComponent* component, const string& name) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::Rename; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	CommandRecord<string> m_record;
};

//////////////////////////////////////////////////////////////////

class SetSourceCommand : public Command
{
public:
	SetSourceCommand(ImageGrid1* component, const Rectangle& source) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetSource; }
	virtual void PostMerge(unique_ptr<Command> other) noexcept override;

private:
	ImageGrid1* m_imgGrid1;
	CommandRecord<Rectangle> m_record;
};

class SetSource39Command : public Command
{
public:
	SetSource39Command(const ImageGrid39Variant& imgGridVariant, const vector<Rectangle>& sources) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetSource39; }
	virtual void PostMerge(unique_ptr<Command> other) noexcept override;

private:
	optional<vector<Rectangle>> GetSources() noexcept;
	bool SetSources(const vector<Rectangle>& sources) noexcept;

	ImageGrid39Variant m_imgGridVariant;
	CommandRecord<vector<Rectangle>> m_record;
};

struct IRenderer;
class SetFilenameCommand : public Command
{
public:
	SetFilenameCommand(const ImageGridVariant& imgGridVariant, IRenderer* renderer, const wstring& filename) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetFilename; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	wstring GetFilename() const noexcept;
	bool SetFilename(const wstring& filename);

	ImageGridVariant m_imgGridVariant;
	IRenderer* m_renderer;
	CommandRecord<wstring> m_record;
};

class SetSourceAndDividerCommand : public Command
{
public:
	SetSourceAndDividerCommand(const ImageGrid39Variant& imgGridVariant, const SourceDivider& srcDivider) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetSourceAndDivider; }
	virtual void PostMerge(unique_ptr<Command> other) noexcept override;

private:
	SourceDivider GetSourceAndDivider() const noexcept;
	bool SetSourceAndDivider(const SourceDivider& srcDivider) noexcept;

	ImageGrid39Variant m_imgGridVariant;
	CommandRecord<SourceDivider> m_record;
};

class TextArea;
class SetTextCommand : public Command
{
public:
	SetTextCommand(TextArea* textArea, const wstring& text) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual CommandID GetTypeID() const noexcept override { return CommandID::SetText; }
	virtual bool IsMerge(Command*) noexcept { return false; }

private:
	CommandRecord<wstring> m_record;
	TextArea* m_textArea;
};