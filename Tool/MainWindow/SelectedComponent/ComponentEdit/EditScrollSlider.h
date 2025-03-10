#pragma once
#include "EditWindow.h"

class ScrollSlider;
class EditScrollSlider : public EditWindow
{
public:
	~EditScrollSlider();
	EditScrollSlider() = delete;
	EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, CommandList* cmdList) noexcept;

private:
	ScrollSlider* m_scrollSlider;
};