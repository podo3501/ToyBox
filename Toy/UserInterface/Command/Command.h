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
	virtual bool Undo() = 0; //Undo와 Redo는 false를 리턴할 가능성이 별로 없다. 하지만 리소스가 삭제되었다거나 해서 false가 나올수 있다.
	virtual bool Redo() = 0;
	virtual unique_ptr<Command> Merge(unique_ptr<Command> other) noexcept;

protected:
	Command(UIComponent* component) : m_component{ component } {} //자식을 통하지 않고서는 객체의 생성을 할 수 없다.
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
