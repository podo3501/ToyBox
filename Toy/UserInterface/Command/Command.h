#pragma once

class UIComponent;

enum class CommandID : int
{
	AttachComponent,
	SetRelativePosition,
	SetSize,
	Rename,
	SetSource,
	SetFilename,
	SetSourceAndDivider,
	Unknown
};

class Command
{
public:
	virtual ~Command() = default;
	
	virtual bool Execute() = 0;
	virtual bool Undo() = 0; //Undo�� Redo�� false�� ������ ���ɼ��� ���� ����. ������ ���ҽ��� �����Ǿ��ٰų� �ؼ� false�� ���ü� �ִ�.
	virtual bool Redo() = 0;
	virtual unique_ptr<Command> Merge(unique_ptr<Command> other) noexcept;

protected:
	Command(UIComponent* component) : m_component{ component } {} //�ڽ��� ������ �ʰ��� ��ü�� ������ �� �� ����.
	virtual CommandID GetTypeID() const noexcept = 0;

	virtual bool IsMerge(Command* other) noexcept;
	virtual void PostMerge(unique_ptr<Command>) noexcept {};
	inline UIComponent* GetComponent() noexcept { return m_component; }
	
private:
	UIComponent* m_component;
};

inline unique_ptr<Command> Command::Merge(unique_ptr<Command> other) noexcept
{
	if (!IsMerge(other.get())) return other;

	PostMerge(move(other));
	return nullptr;
}

inline bool Command::IsMerge(Command* other) noexcept
{
	if (GetTypeID() != other->GetTypeID()) return false;
	if (m_component != other->m_component) return false;

	return true;
}
