#include "pch.h"
#include "EditScrollSlider.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollSlider.h"

EditScrollSlider::~EditScrollSlider() = default;
EditScrollSlider::EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ scrollSlider, sourceBinder, cmdList },
	m_scrollSlider{ scrollSlider }
{
}