#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class ImageSwitcher;
class EditCombo;
class EditImageSwitcher : public EditWindow
{
public:
	~EditImageSwitcher();
	EditImageSwitcher() = delete;
	EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	ImageSwitcher* m_imageSwitcher;
	unique_ptr<EditCombo> m_combo;
};