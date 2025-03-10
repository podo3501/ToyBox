#include "pch.h"
#include "EditScrollBar.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"

EditScrollBar::~EditScrollBar() = default;
EditScrollBar::EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ scrollBar, cmdList },
	m_scrollBar{ scrollBar }
{
}