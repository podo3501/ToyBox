#pragma once

enum class CommandID : int
{
	RelativePosition,
	Size,
	Rename,
	Source,
	Unknown
};

class Command
{
public:
	virtual ~Command() {};
	virtual CommandID GetTypeID() const noexcept = 0;

	virtual bool Execute() = 0;
	virtual bool Redo() = 0;
	virtual bool Undo() = 0;
	virtual bool IsMerge() noexcept { return false; }
	virtual void Merge(unique_ptr<Command>) noexcept {}

protected:
	Command() = default; //자식을 통하지 않고서는 만들 수 없다.
};

template <typename TargetType>
TargetType CommandCast(Command* command)
{
	if (!command) return nullptr;
	CommandID typeId = command->GetTypeID();
	if (typeId == CommandID::Unknown) return nullptr;

	using NonPointerTargetType = typename std::remove_pointer<TargetType>::type; //Target타입이 Button* 이렇게 포인터로 넘어오기 때문에 *를 떼어냄
	if (typeId == NonPointerTargetType::GetTypeStatic())
		return static_cast<TargetType>(command);

	return nullptr;
}
