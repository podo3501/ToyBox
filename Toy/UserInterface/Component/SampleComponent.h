#pragma once
#include "../UIComponent.h"
#include "../UILayout.h"

struct IRenderer;
class ImageGrid1;
class ImageGrid3;
class ImageGrid9;
class ListArea;
class ScrollBar;
class ScrollSlider;
class Button;
class TextArea;
class Container;
class ImageSwitcher;
enum class DirectionType;

unique_ptr<ImageGrid1> CreateSampleImageGrid1(const UILayout& layout);
unique_ptr<ImageGrid3> CreateSampleImageGrid3(DirectionType dirType, const UILayout& layout);
unique_ptr<ImageGrid9> CreateSampleImageGrid9(const UILayout& layout);
unique_ptr<TextArea> CreateSampleTextArea(const UILayout& layout, const wstring& text);
unique_ptr<Button> CreateSampleButton1(const UILayout& layout);
unique_ptr<Button> CreateSampleButton3(DirectionType dirtype, const UILayout& layout);
unique_ptr<ImageSwitcher> CreateSampleImageSwitcher1(const UILayout& layout, BehaviorMode behavior);
unique_ptr<ImageSwitcher> CreateSampleImageSwitcher3(const UILayout& layout, DirectionType dirType, BehaviorMode behavior);
unique_ptr<ImageGrid1> CreateListBackgroudImage(const UILayout& layout);
unique_ptr<ListArea> CreateSampleListArea1(const UILayout& layout);
unique_ptr<Container> CreateSampleListContainer(const UILayout& layout);
unique_ptr<ImageGrid3> CreateScrollTrack(DirectionType dirType, const UILayout& layout);
unique_ptr<Container> CreateScrollContainer(const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, ListArea* listArea, int itemCount);

///////////////////////////////////////////////////////////////

inline map<InteractState, string> GetStateKeyMap(const string& prefix) noexcept
{
	return {
		{ InteractState::Normal, prefix + "_" + EnumToString(InteractState::Normal) },
		{ InteractState::Hovered, prefix + "_" + EnumToString(InteractState::Hovered) },
		{ InteractState::Pressed, prefix + "_" + EnumToString(InteractState::Pressed) } };
}

map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(const XMUINT2& size, const string& bindKey);
map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(DirectionType dirType, const XMUINT2& size, const string& bindKey);

unique_ptr<ScrollSlider> CreateSampleScrollSlider(DirectionType dirType, const UILayout& layout);
unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout);
unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, TextureSourceBinder* sb, ListArea* listArea, int itemCount);
