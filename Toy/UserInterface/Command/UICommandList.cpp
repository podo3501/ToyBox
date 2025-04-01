#include "pch.h"
#include "UICommandList.h"
#include "Command.h"
#include "CommandRegistry.h"
#include "../UIComponent/UIComponent.h"
#include "../../Utility.h"
#include "../UIComponent/UIType.h"

UICommandList::~UICommandList() = default;
UICommandList::UICommandList() = default;

bool UICommandList::Undo() noexcept
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

bool UICommandList::Redo() noexcept
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

unique_ptr<Command> UICommandList::TryMergeCommand(unique_ptr<Command> cmd) noexcept
{
	if (m_commandList.empty()) return cmd;

	auto& preCmd = m_commandList.back();
	return preCmd->Merge(move(cmd));
}

void UICommandList::AddOrMergeCommand(unique_ptr<Command> command) noexcept
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
bool UICommandList::ApplyCommand(ParamTypes&&... params)
{
	unique_ptr<Command> command = make_unique<CommandType>(forward<ParamTypes>(params)...);
	ReturnIfFalse(command->Execute());

	AddOrMergeCommand(move(command));
	return true;
}

unique_ptr<UIComponent> UICommandList::AttachComponent(UIComponent* addable, 
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

pair<unique_ptr<UIComponent>, UIComponent*> UICommandList::DetachComponent(UIComponent* detach)
{
	unique_ptr<Command> command = make_unique<DetachComponentCommand>(detach);
	command->Execute();
	
	auto detachCmd = static_cast<DetachComponentCommand*>(command.get());
	auto [detached, parent] = detachCmd->GetResult();
	if (detached)
		AddOrMergeCommand(move(command));

	return { move(detached), parent };
}

bool UICommandList::SetRelativePosition(UIComponent* component, const XMINT2& relativePos)
{
	return ApplyCommand<SetRelativePositionCommand>(component, relativePos);
}

bool UICommandList::SetSize(UIComponent* component, const XMUINT2& size)
{
	return ApplyCommand<SetSizeCommand>(component, size);
}

bool UICommandList::RenameRegion(UIComponent* component, const string& region)
{
	return ApplyCommand<RenameRegionCommand>(component, region);
}

bool UICommandList::Rename(UIComponent* component, const string& name)
{
	return ApplyCommand<RenameCommand>(component, name);
}

//bool UICommandList::SetSource(ImageGrid1* imgGrid1, const Rectangle& source)
//{
//	return ApplyCommand<SetSourceCommand>(imgGrid1, source);
//}
////Variant�� �Ⱦ��� �׳� UIComponent* �� �޾Ƽ� ó���ҷ��� �ߴµ� ���� �ٸ��� �ʾҴ�.
////������ �ƹ� Component�� ������ �λ��� �ֱ� ������ �װ� �ɷ����� �߰����� �۾��� �����.
//bool UICommandList::SetSources(const ImageGrid39Variant& imgGridVariant, const vector<Rectangle>& sources)
//{
//	return ApplyCommand<SetSource39Command>(imgGridVariant, sources);
//}

//bool UICommandList::SetFilename(const ImageGridVariant& imgGridVariant, IRenderer* renderer, const wstring& filename)
//{
//	return ApplyCommand<SetFilenameCommand>(imgGridVariant, renderer, filename);
//}

//bool UICommandList::SetSourceAndDivider(const ImageGrid39Variant& imgGridVariant, const SourceDivider& srcDivider)
//{
//	return ApplyCommand<SetSourceAndDividerCommand>(imgGridVariant, srcDivider);
//}

bool UICommandList::SetText(TextArea* textArea, const wstring& text)
{
	return ApplyCommand<SetTextCommand>(textArea, text);
}