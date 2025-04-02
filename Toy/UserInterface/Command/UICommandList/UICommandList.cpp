#include "pch.h"
#include "UICommandList.h"
#include "UICommandRegistry.h"
#include "../../UIComponent/UIComponent.h"
#include "../../../Utility.h"
#include "../../UIComponent/UIType.h"

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
////Variant를 안쓰고 그냥 UIComponent* 로 받아서 처리할려고 했는데 별반 다르지 않았다.
////오히려 아무 Component가 들어오는 인상을 주기 때문에 그걸 걸러내는 추가적인 작업만 생긴다.
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