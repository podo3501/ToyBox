#pragma once
#include "EditWindow.h"

class ScrollBar;
class TextureSourceBinder;
class EditScrollBar : public EditWindow
{
public:
	~EditScrollBar();
	EditScrollBar() = delete;
	EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;

private:
	ScrollBar* m_scrollBar;
};