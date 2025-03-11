#pragma once
#include "EditWindow.h"

class ImageSwitcher;
class EditImageSwitcher : public EditWindow
{
public:
	~EditImageSwitcher();
	EditImageSwitcher() = delete;
	EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, CommandList* cmdList) noexcept;

private:
	ImageSwitcher* m_imageSwitcher;
};