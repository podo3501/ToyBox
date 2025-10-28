#pragma once

struct IRenderer;
class UIComponent;
class UILayout;
class ListArea;
class ScrollBar;
class TextureResourceBinder;
enum class InteractState;
enum class DirectionType;

map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(const XMUINT2& size, const string& bindKey);
map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(DirectionType dirType, const XMUINT2& size, const string& bindKey);

unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout, DirectionType dirType);
unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, TextureResourceBinder* rb, ListArea* listArea, int itemCount);
