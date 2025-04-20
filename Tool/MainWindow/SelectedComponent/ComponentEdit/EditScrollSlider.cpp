#include "pch.h"
#include "EditScrollSlider.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollSlider.h"

EditScrollSlider::~EditScrollSlider() = default;
EditScrollSlider::EditScrollSlider(ScrollSlider* scrollSlider, UICommandList* cmdList) noexcept :
	EditWindow{ scrollSlider, cmdList },
	m_scrollSlider{ scrollSlider }
{
}