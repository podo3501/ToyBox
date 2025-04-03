#pragma once
#include "EditWindow.h"

class ScrollBar;
class TextureResourceBinder;
class EditScrollBar : public EditWindow
{
public:
	~EditScrollBar();
	EditScrollBar() = delete;
	EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	ScrollBar* m_scrollBar;
};