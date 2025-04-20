#include "pch.h"
#include "EditListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"

EditListArea::~EditListArea() = default;
EditListArea::EditListArea(ListArea* listArea, UICommandList* cmdList) noexcept :
	EditWindow{ listArea, cmdList },
	m_listArea{ listArea }
{
}