#pragma once
#include "CommandType.h"

struct IRenderer;
class Command;
class UIComponent;
class TextArea;
struct SourceDivider;

class UICommandList
{
public:
	~UICommandList();
	UICommandList();

	bool Undo() noexcept;
	bool Redo() noexcept;

	unique_ptr<UIComponent> AttachComponent(UIComponent* addable, unique_ptr<UIComponent> component, const XMINT2& relativePos);
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* detach);
	bool SetRelativePosition(UIComponent* component, const XMINT2& relativePos);
	bool SetSize(UIComponent* component, const XMUINT2& size);
	bool RenameRegion(UIComponent* component, const string& region);
	bool Rename(UIComponent* component, const string& name);
	//bool SetSource(ImageGrid1* imgGrid1, const Rectangle& source);
	//bool SetSources(const ImageGrid39Variant& imgGridVariant, const vector<Rectangle>& sources);
	//bool SetFilename(const ImageGridVariant& imgGridVariant, IRenderer* renderer, const wstring& filename);
	//bool SetSourceAndDivider(const ImageGrid39Variant& imgGridVariant, const SourceDivider& srcDivider);
	bool SetText(TextArea* textArea, const wstring& text);

private:
	unique_ptr<Command> TryMergeCommand(unique_ptr<Command> cmd) noexcept;
	void AddOrMergeCommand(unique_ptr<Command> command) noexcept;
	template <typename CommandType, typename... ParamTypes>
	bool ApplyCommand(ParamTypes&&... params);

	vector<unique_ptr<Command>> m_commandList;
	int m_index{ -1 };
};