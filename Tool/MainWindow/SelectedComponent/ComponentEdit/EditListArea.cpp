#include "pch.h"
#include "EditListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"

EditListArea::~EditListArea() = default;
EditListArea::EditListArea(ListArea* listArea, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ listArea, sourceBinder, cmdList },
	m_listArea{ listArea }
{
}