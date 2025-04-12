#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class TextureSwitcher;
class EditCombo;
class EditTextureSwitcher : public EditWindow
{
public:
	~EditTextureSwitcher();
	EditTextureSwitcher() = delete;
	EditTextureSwitcher(TextureSwitcher* texSwitcher, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	void RenderStateCombo();
	void RenderKeyCombo();
	void RenderFitToTextureButton();
	void SelectKeyComboItem();


	TextureSwitcher* m_texSwitcher;
	unique_ptr<EditCombo> m_stateCombo;
	unique_ptr<EditCombo> m_keyCombo;
};