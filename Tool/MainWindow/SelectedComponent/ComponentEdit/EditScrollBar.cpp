#include "pch.h"
#include "EditScrollBar.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"

EditScrollBar::~EditScrollBar() = default;
EditScrollBar::EditScrollBar(ScrollBar* scrollBar, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ scrollBar, sourceBinder, cmdList },
	m_scrollBar{ scrollBar }
{
}