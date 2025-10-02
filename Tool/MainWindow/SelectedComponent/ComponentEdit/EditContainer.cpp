#include "pch.h"
#include "EditContainer.h"
#include "Toy/UserInterface/UIComponent/Components/Container.h"

EditContainer::~EditContainer() = default;
EditContainer::EditContainer(Container* textArea, UICommandList* cmdList) noexcept :
	EditWindow{ textArea, cmdList },
	m_container{ nullptr }
{
}

void EditContainer::RenderComponent()
{
}