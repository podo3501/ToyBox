#include "pch.h"
#include "EditScrollBar.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollBar.h"

EditScrollBar::~EditScrollBar() = default;
EditScrollBar::EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
	EditWindow{ scrollBar, sourceBinder, cmdList },
	m_scrollBar{ scrollBar }
{
}