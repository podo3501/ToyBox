#pragma once
#include "EditWindow.h"

class ScrollSlider;
class TextureResourceBinder;
class EditScrollSlider : public EditWindow
{
public:
	~EditScrollSlider();
	EditScrollSlider() = delete;
	EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	ScrollSlider* m_scrollSlider;
};