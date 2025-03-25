#include "pch.h"
#include "CommandList.h"
#include "Command.h"
#include "CommandRegistry.h"
#include "../UIComponent.h"
#include "../../Utility.h"
#include "../UIType.h"

CommandList::~CommandList() = default;
CommandList::CommandList() = default;

bool CommandList::Undo() noexcept
{
	if (m_index < 0) return true;

	auto& cmd = m_commandList[m_index];
	auto result = cmd->Undo();
	
	if (!result)
	{
		AssertMsg(false, "Undo�Լ��� �����ϴ� ���� ������ ���̱� ������ �����ؼ��� �ȵȴ�.");
		return result;
	}

	m_index--;
	return true;
}

bool CommandList::Redo() noexcept
{
	if (m_index + 1 >= static_cast<int>(m_commandList.size())) return true;

	auto& cmd = m_commandList[m_index + 1];
	auto result = cmd->Redo();

	if (!result)
	{
		AssertMsg(false, "Redo�Լ��� �����ߴ� ���� �ٽ� �����ϴ� ���̱� ������ �����ؼ��� �ȵȴ�.");
		return result;
	}

	m_index++;
	return true;
}

unique_ptr<Command> CommandList::TryMergeCommand(unique_ptr<Command> cmd) noexcept
{
	if (m_commandList.empty()) return cmd;

	auto& preCmd = m_commandList.back();
	return preCmd->Merge(move(cmd));
}

void CommandList::AddOrMergeCommand(unique_ptr<Command> command) noexcept
{
	if (m_index < static_cast<int>(m_commandList.size()) - 1)
		m_commandList.resize(m_index + 1);

	//Merge�� �õ��ϰ� �����ϸ� command�� �ٽ� �����ش�. 
	// �ƴϸ� IsMerge�� �ҷ���� �ϴµ� �ִ��� public �Լ��� �� �����°� ���� �ڵ����̴�.
	command = TryMergeCommand(move(command));
	if (!command) return;

	m_commandList.emplace_back(move(command));
	m_index++;
}

template <typename CommandType, typename... ParamTypes>
bool CommandList::ApplyCommand(ParamTypes&&... params)
{
	unique_ptr<Command> command = make_unique<CommandType>(forward<ParamTypes>(params)...);
	ReturnIfFalse(command->Execute());

	AddOrMergeCommand(move(command));
	return true;
}

unique_ptr<UIComponent> CommandList::AttachComponent(UIComponent* addable, 
	unique_ptr<UIComponent> component, const XMINT2& relativePos)
{
	unique_ptr<Command> command = make_unique<AttachComponentCommand>(addable, move(component), relativePos);
	if (!command->Execute())
	{
		auto attachCmd = static_cast<AttachComponentCommand*>(command.get());
		return attachCmd->GetFailureResult();
	}

	AddOrMergeCommand(move(command));
	return nullptr;
}

pair<unique_ptr<UIComponent>, UIComponent*> CommandList::DetachComponent(UIComponent* detach)
{
	unique_ptr<Command> command = make_unique<DetachComponentCommand>(detach);
	command->Execute();
	
	auto detachCmd = static_cast<DetachComponentCommand*>(command.get());
	auto [detached, parent] = detachCmd->GetResult();
	if (detached)
		AddOrMergeCommand(move(command));

	return { move(detached), parent };
}

bool CommandList::SetRelativePosition(UIComponent* component, const XMINT2& relativePos)
{
	return ApplyCommand<SetRelativePositionCommand>(component, relativePos);
}

bool CommandList::SetSize(UIComponent* component, const XMUINT2& size)
{
	return ApplyCommand<SetSizeCommand>(component, size);
}

bool CommandList::RenameRegion(UIComponent* component, const string& region)
{
	return ApplyCommand<RenameRegionCommand>(component, region);
}

bool CommandList::Rename(UIComponent* component, const string& name)
{
	return ApplyCommand<RenameCommand>(component, name);
}

bool CommandList::SetSource(ImageGrid1* imgGrid1, const Rectangle& source)
{
	return ApplyCommand<SetSourceCommand>(imgGrid1, source);
}
//Variant�� �Ⱦ��� �׳� UIComponent* �� �޾Ƽ� ó���ҷ��� �ߴµ� ���� �ٸ��� �ʾҴ�.
//������ �ƹ� Component�� ������ �λ��� �ֱ� ������ �װ� �ɷ����� �߰����� �۾��� �����.
bool CommandList::SetSources(const ImageGrid39Variant& imgGridVariant, const vector<Rectangle>& sources)
{
	return ApplyCommand<SetSource39Command>(imgGridVariant, sources);
}

bool CommandList::SetFilename(const ImageGridVariant& imgGridVariant, IRenderer* renderer, const wstring& filename)
{
	return ApplyCommand<SetFilenameCommand>(imgGridVariant, renderer, filename);
}

bool CommandList::SetSourceAndDivider(const ImageGrid39Variant& imgGridVariant, const SourceDivider& srcDivider)
{
	return ApplyCommand<SetSourceAndDividerCommand>(imgGridVariant, srcDivider);
}

bool CommandList::SetText(TextArea* textArea, const wstring& text)
{
	return ApplyCommand<SetTextCommand>(textArea, text);
}