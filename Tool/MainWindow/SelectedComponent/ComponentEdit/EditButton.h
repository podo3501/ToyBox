#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class Button;
class EditButton : public EditWindow
{
public:
	~EditButton();
	EditButton() = delete;
	EditButton(Button* button, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	Button* m_button;
};