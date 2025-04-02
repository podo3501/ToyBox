#include "pch.h"
#include "EditContainer.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"

EditContainer::~EditContainer() = default;
EditContainer::EditContainer(Container* textArea, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
	EditWindow{ textArea, sourceBinder, cmdList },
	m_container{ nullptr }
{
}

void EditContainer::RenderComponent()
{
}