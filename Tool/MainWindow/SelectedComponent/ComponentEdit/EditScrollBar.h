#pragma once
#include "EditWindow.h"

class ScrollBar;
class EditScrollBar : public EditWindow
{
public:
	~EditScrollBar();
	EditScrollBar() = delete;
	EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, CommandList* cmdList) noexcept;

private:
	ScrollBar* m_scrollBar;
};