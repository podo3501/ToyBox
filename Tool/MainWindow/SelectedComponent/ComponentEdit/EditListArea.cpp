#include "pch.h"
#include "EditListArea.h"
#include "../Toy/UserInterface/Component/ListArea.h"

EditListArea::~EditListArea() = default;
EditListArea::EditListArea(ListArea* listArea, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ listArea, cmdList },
	m_listArea{ listArea }
{
}