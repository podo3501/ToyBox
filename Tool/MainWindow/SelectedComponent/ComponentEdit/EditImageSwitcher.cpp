#include "pch.h"
#include "EditImageSwitcher.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"

EditImageSwitcher::~EditImageSwitcher() = default;
EditImageSwitcher::EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ imageSwitcher, cmdList },
	m_imageSwitcher{ imageSwitcher }
{
}