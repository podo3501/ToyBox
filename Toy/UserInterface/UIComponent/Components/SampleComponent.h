#pragma once

struct IRenderer;
class UIComponent;
class UILayout;
class ListArea;
class ScrollBar;
class ScrollSlider;
class TextureSourceBinder;
enum class InteractState;
enum class DirectionType;

map<InteractState, string> GetStateKeyMap(const string& prefix) noexcept;
map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(const XMUINT2& size, const string& bindKey);
map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(DirectionType dirType, const XMUINT2& size, const string& bindKey);

unique_ptr<ScrollSlider> CreateSampleScrollSlider(DirectionType dirType, const UILayout& layout);
unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout);
unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout);

bool MakeSampleListAreaData(IRenderer* renderer, TextureSourceBinder* sb, ListArea* listArea, int itemCount);
