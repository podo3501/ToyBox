#include "pch.h"
#include "EditListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"

EditListArea::~EditListArea() = default;
EditListArea::EditListArea(ListArea* listArea, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ listArea, resBinder, cmdList },
	m_listArea{ listArea }
{
}