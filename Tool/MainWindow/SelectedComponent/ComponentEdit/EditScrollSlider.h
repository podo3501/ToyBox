#pragma once
#include "EditWindow.h"

class ScrollSlider;
class TextureSourceBinder;
class EditScrollSlider : public EditWindow
{
public:
	~EditScrollSlider();
	EditScrollSlider() = delete;
	EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept;

private:
	ScrollSlider* m_scrollSlider;
};