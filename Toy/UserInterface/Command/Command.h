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
	Command() = default; //�ڽ��� ������ �ʰ��� ���� �� ����.
};

template <typename TargetType>
TargetType CommandCast(Command* command)
{
	if (!command) return nullptr;
	CommandID typeId = command->GetTypeID();
	if (typeId == CommandID::Unknown) return nullptr;

	using NonPointerTargetType = typename std::remove_pointer<TargetType>::type; //TargetŸ���� Button* �̷��� �����ͷ� �Ѿ���� ������ *�� ���
	if (typeId == NonPointerTargetType::GetTypeStatic())
		return static_cast<TargetType>(command);

	return nullptr;
}
