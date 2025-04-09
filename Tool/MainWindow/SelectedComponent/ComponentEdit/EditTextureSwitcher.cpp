#include "pch.h"
#include "EditTextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "EditUtility/EditUtility.h"

EditTextureSwitcher::~EditTextureSwitcher() = default;
EditTextureSwitcher::EditTextureSwitcher(TextureSwitcher* texSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ texSwitcher, resBinder, cmdList },
	m_texSwitcher{ texSwitcher },
	m_combo{ make_unique<EditCombo>("State", EnumToList<InteractState>()) }
{}

void EditTextureSwitcher::RenderComponent()
{
	m_combo->Render([](const string& key) {

		});
}