#pragma once
#include<variant>

class ImageGrid1;
class ImageGrid3;
class ImageGrid9;

using ImageGridVariant = std::variant<ImageGrid1*, ImageGrid3*, ImageGrid9*>;
using ImageGrid39Variant = std::variant<ImageGrid3*, ImageGrid9*>;

enum class UICommandID : int
{
	AttachComponent,
	DetachComponent,
	SetRelativePosition,
	SetSize,
	SetRegion,
	Rename,
	SetSource,
	SetSource39,
	SetFilename,
	SetSourceAndDivider,
	SetText,
	Unknown
};
class UIComponent;
using UICommand = Command<UICommandID, UIComponent>;

enum class TexSrcCommandID : int
{
	AddFontKey,
	RemoveFontKey,
	AddTextureKey,
	RemoveTextureKey,
	RemoveKeyByFilename,
	RenameFontKey,
	RenameTextureKey,
	ModifyTexSrcInfo,
	Unknown
};
class TextureSourceBinder;
using TexSrcCommand = Command<TexSrcCommandID, TextureSourceBinder>;

template <typename T>
class CommandRecord
{
public:
	CommandRecord() noexcept = default;
	CommandRecord(const T& curr) noexcept : current{ curr } {}
	CommandRecord(T&& curr) noexcept : current(move(curr)) {}

	T current{};
	T previous{};
};