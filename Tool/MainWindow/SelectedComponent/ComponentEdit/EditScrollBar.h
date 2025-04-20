#pragma once
#include "EditWindow.h"

class ScrollBar;
class TextureResourceBinder;
class EditScrollBar : public EditWindow
{
public:
	~EditScrollBar();
	EditScrollBar() = delete;
	EditScrollBar(ScrollBar* scrollBar, UICommandList* cmdList) noexcept;

private:
	ScrollBar* m_scrollBar;
};