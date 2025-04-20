#pragma once
#include "EditWindow.h"

class ScrollSlider;
class TextureResourceBinder;
class EditScrollSlider : public EditWindow
{
public:
	~EditScrollSlider();
	EditScrollSlider() = delete;
	EditScrollSlider(ScrollSlider* scrollSlider, UICommandList* cmdList) noexcept;

private:
	ScrollSlider* m_scrollSlider;
};