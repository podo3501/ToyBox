#include "pch.h"
#include "EditScrollBar.h"
#include "UserInterface/UIComponent/Components/ScrollBar.h"

EditScrollBar::~EditScrollBar() = default;
EditScrollBar::EditScrollBar(ScrollBar* scrollBar, UICommandList* cmdList) noexcept :
	EditWindow{ scrollBar, cmdList },
	m_scrollBar{ scrollBar }
{
}