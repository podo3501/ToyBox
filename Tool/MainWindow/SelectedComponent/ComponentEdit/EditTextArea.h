#pragma once
#include "EditWindow.h"

class TextArea;
class TextureResourceBinder;
class EditTextArea : public EditWindow
{
public:
	~EditTextArea();
	EditTextArea() = delete;
	EditTextArea(TextArea* textArea, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	TextArea* m_textArea;
};