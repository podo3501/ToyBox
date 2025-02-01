#pragma once
#include "../EditWindow.h"

class Button;
class EditButton : public EditWindow
{
public:
	~EditButton();
	EditButton() = delete;
	EditButton(Button* button, IRenderer* renderer, CommandList* cmdList) noexcept;

protected:
	virtual void UpdateComponent() override;
	virtual void RenderComponent() override;

private:
	Button* m_button;
};