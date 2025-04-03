#include "pch.h"
#include "EditContainer.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"

EditContainer::~EditContainer() = default;
EditContainer::EditContainer(Container* textArea, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ textArea, resBinder, cmdList },
	m_container{ nullptr }
{
}

void EditContainer::RenderComponent()
{
}