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
enum class DirectionType;

unique_ptr<ImageGrid1> CreateSampleImageGrid1(const UILayout& layout);
unique_ptr<ImageGrid3> CreateSampleImageGrid3(DirectionType dirType, const UILayout& layout);
unique_ptr<ImageGrid9> CreateSampleImageGrid9(const UILayout& layout);
unique_ptr<TextArea> CreateSampleTextArea(const UILayout& layout, const wstring& text);
unique_ptr<Button> CreateSampleButton1(const UILayout& layout);
unique_ptr<Button> CreateSampleButton3(DirectionType dirtype, const UILayout& layout);
unique_ptr<ImageGrid1> CreateListBackgroudImage(const UILayout& layout);
unique_ptr<ListArea> CreateSampleListArea1(const UILayout& layout);
unique_ptr<Container> CreateSampleListContainer(const UILayout& layout);
unique_ptr<ImageGrid3> CreateScrollTrack(DirectionType dirType, const UILayout& layout);
unique_ptr<Container> CreateScrollContainer(const UILayout& layout);
unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout);
unique_ptr<ScrollSlider> CreateSampleScrollSlider(DirectionType dirType, const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, ListArea* listArea, int itemCount);