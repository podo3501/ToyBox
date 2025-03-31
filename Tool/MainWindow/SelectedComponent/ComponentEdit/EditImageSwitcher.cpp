#include "pch.h"
#include "EditImageSwitcher.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"

EditImageSwitcher::~EditImageSwitcher() = default;
EditImageSwitcher::EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ imageSwitcher, sourceBinder, cmdList },
	m_imageSwitcher{ imageSwitcher }
{
}