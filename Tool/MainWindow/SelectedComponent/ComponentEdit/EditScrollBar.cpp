#include "pch.h"
#include "EditScrollBar.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollBar.h"

EditScrollBar::~EditScrollBar() = default;
EditScrollBar::EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ scrollBar, resBinder, cmdList },
	m_scrollBar{ scrollBar }
{
}