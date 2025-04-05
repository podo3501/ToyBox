#include "pch.h"
#include "EditImageSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageSwitcher.h"
#include "EditUtility/EditUtility.h"

EditImageSwitcher::~EditImageSwitcher() = default;
EditImageSwitcher::EditImageSwitcher(ImageSwitcher* imageSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ imageSwitcher, resBinder, cmdList },
	m_imageSwitcher{ imageSwitcher },
	m_combo{ make_unique<EditCombo>("State", EnumToList<InteractState>()) }
{}

void EditImageSwitcher::RenderComponent()
{
	m_combo->Render([](const string& key) {

		});
}