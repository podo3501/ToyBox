#pragma once

struct IRenderer;
class Command;
class UIComponent;
class ImageGrid1;
class ImageGrid3;
class ImageGrid9;
struct SourceDivider;

class CommandList
{
	using ImageGridVariant = variant<ImageGrid1*, ImageGrid3*, ImageGrid9*>;
	using ImageGrid39Variant = variant<ImageGrid3*, ImageGrid9*>;

public:
	~CommandList();
	CommandList();

	bool Undo() noexcept;
	bool Redo() noexcept;

	unique_ptr<UIComponent> AttachComponent(UIComponent* addable, unique_ptr<UIComponent> component, const XMINT2& relativePos);
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* detach);
	bool SetRelativePosition(UIComponent* component, const XMINT2& relativePos);
	bool SetSize(UIComponent* component, const XMUINT2& size);
	bool Rename(UIComponent* component, const string& name);
	bool SetSource(ImageGrid1* imgGrid1, const Rectangle& source);
	bool SetFilename(const ImageGridVariant& imgGridVariant, IRenderer* renderer, const wstring& filename);
	bool SetSourceAndDivider(const ImageGrid39Variant& imgGridVariant, const SourceDivider& srcDivider);

private:
	unique_ptr<Command> TryMergeCommand(unique_ptr<Command> cmd) noexcept;
	void AddOrMergeCommand(unique_ptr<Command> command) noexcept;
	template <typename CommandType, typename... ParamTypes>
	bool ApplyCommand(ParamTypes&&... params);

	vector<unique_ptr<Command>> m_commandList;
	int m_index{ -1 };
};