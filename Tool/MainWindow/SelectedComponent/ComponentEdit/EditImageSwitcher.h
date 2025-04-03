#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class ImageSwitcher;
class EditImageSwitcher : public EditWindow
{
public:
	~EditImageSwitcher();
	EditImageSwitcher() = delete;
	EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	ImageSwitcher* m_imageSwitcher;
};