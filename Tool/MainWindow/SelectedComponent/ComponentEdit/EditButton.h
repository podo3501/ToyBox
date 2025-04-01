#pragma once
#include "EditWindow.h"

class TextureSourceBinder;
class Button;
class EditButton : public EditWindow
{
public:
	~EditButton();
	EditButton() = delete;
	EditButton(Button* button, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	Button* m_button;
};