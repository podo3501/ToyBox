#include "pch.h"
#include "EditScrollSlider.h"
#include "../Toy/UserInterface/Component/ScrollSlider.h"

EditScrollSlider::~EditScrollSlider() = default;
EditScrollSlider::EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ scrollSlider, cmdList },
	m_scrollSlider{ scrollSlider }
{
}