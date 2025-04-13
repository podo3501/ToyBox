#include "pch.h"
#include "EditTextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "EditUtility/EditUtility.h"

EditTextureSwitcher::~EditTextureSwitcher() = default;
EditTextureSwitcher::EditTextureSwitcher(TextureSwitcher* texSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ texSwitcher, resBinder, cmdList },
	m_texSwitcher{ texSwitcher }
{
	if (auto state = m_texSwitcher->GetState(); state)
	{
		m_stateCombo = make_unique<EditCombo>("State", EnumToList<InteractState>());
		m_stateCombo->SelectItem(EnumToString(*state));
	}

	if (auto bindKey = texSwitcher->GetBindKey(); bindKey)
	{
		const auto& keys = resBinder->GetTextureKeys(GetTextureSlice(texSwitcher));
		m_keyCombo = make_unique<EditCombo>("Bind Keys", keys);
		m_keyCombo->SelectItem(*bindKey);
	}
}

void EditTextureSwitcher::SelectKeyComboItem()
{
	if (auto bindKey = m_texSwitcher->GetBindKey(); bindKey)
		m_keyCombo->SelectItem(*bindKey);
}

void EditTextureSwitcher::RenderComponent()
{
	RenderFitToTextureButton();
	RenderStateCombo();
	RenderKeyCombo();
}

void EditTextureSwitcher::RenderStateCombo()
{
	if (!m_stateCombo) return;

	m_stateCombo->Render([this](const std::string& key) {
		m_texSwitcher->ChangeState(StringToEnum<InteractState>(key));
		SelectKeyComboItem();
		});
}

void EditTextureSwitcher::RenderKeyCombo()
{
	if (!m_keyCombo) return;

	m_keyCombo->Render([this](const std::string& key) {
		m_texSwitcher->ChangeBindKey(GetTextureResourceBinder(), key);
		});
}

void EditTextureSwitcher::RenderFitToTextureButton()
{
	if (m_stateCombo && m_keyCombo)
	{
		if (ImGui::Button("Fit to Texture Size"))
			m_texSwitcher->FitToTextureSource();
	}
}
