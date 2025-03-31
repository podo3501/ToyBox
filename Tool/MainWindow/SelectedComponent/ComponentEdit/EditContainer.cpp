#include "pch.h"
#include "EditContainer.h"
#include "../Toy/UserInterface/Component/Container.h"
#include "../Toy/UserInterface/Command/CommandList.h"

EditContainer::~EditContainer() = default;
EditContainer::EditContainer(Container* textArea, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ textArea, sourceBinder, cmdList },
	m_container{ nullptr }
{
}

void EditContainer::RenderComponent()
{
}