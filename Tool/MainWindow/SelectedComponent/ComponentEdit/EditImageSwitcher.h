#pragma once
#include "EditWindow.h"

class TextureSourceBinder;
class ImageSwitcher;
class EditImageSwitcher : public EditWindow
{
public:
	~EditImageSwitcher();
	EditImageSwitcher() = delete;
	EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;

private:
	ImageSwitcher* m_imageSwitcher;
};