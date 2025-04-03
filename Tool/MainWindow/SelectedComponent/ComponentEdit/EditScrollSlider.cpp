#include "pch.h"
#include "EditScrollSlider.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollSlider.h"

EditScrollSlider::~EditScrollSlider() = default;
EditScrollSlider::EditScrollSlider(ScrollSlider* scrollSlider, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ scrollSlider, resBinder, cmdList },
	m_scrollSlider{ scrollSlider }
{
}