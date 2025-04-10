#include "pch.h"
#include "UICommandList.h"
#include "UICommandRegistry.h"
#include "../../UIComponent/UIComponent.h"
#include "../../UIComponent/UIType.h"
#include "Utility.h"

UICommandList::~UICommandList() = default;
UICommandList::UICommandList() = default;

unique_ptr<UIComponent> UICommandList::AttachComponent(UIComponent* addable, 
	unique_ptr<UIComponent> component, const XMINT2& relativePos)
{
	unique_ptr<UICommand> command = make_unique<AttachComponentCommand>(addable, move(component), relativePos);
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
	unique_ptr<UICommand> command = make_unique<DetachComponentCommand>(detach);
	command->Execute();
	
	auto detachCmd = static_cast<DetachComponentCommand*>(command.get());
	auto [detached, parent] = detachCmd->GetResult();
	if (detached)
		AddOrMergeCommand(move(command));

	return { move(detached), parent };
}

bool UICommandList::SetRelativePosition(UIComponent* component, const XMINT2& relativePos)
{ return ApplyCommand<SetRelativePositionCommand>(component, relativePos); }

bool UICommandList::SetSize(UIComponent* component, const XMUINT2& size)
{ return ApplyCommand<SetSizeCommand>(component, size); }

bool UICommandList::RenameRegion(UIComponent* component, const string& region)
{ return ApplyCommand<RenameRegionCommand>(component, region); }

bool UICommandList::Rename(UIComponent* component, const string& name)
{ return ApplyCommand<RenameCommand>(component, name); }

bool UICommandList::SetText(TextArea* textArea, const wstring& text)
{ return ApplyCommand<SetTextCommand>(textArea, text); }