#pragma once
#include "../UIComponent.h"
#include "../UILayout.h"

struct IRenderer;
class ListArea;

unique_ptr<UIComponent> CreateSampleImageGrid1(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleImageGrid3(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleImageGrid9(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleTextArea(const UILayout& layout, const wstring& text);
unique_ptr<UIComponent> CreateSampleButton1(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleButton3(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleListArea1(const UILayout& layout);
unique_ptr<UIComponent> CreateSampleScrollBar(const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, ListArea* listArea);