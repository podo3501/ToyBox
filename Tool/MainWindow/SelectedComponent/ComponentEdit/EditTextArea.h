#pragma once
#include "EditWindow.h"

class TextArea;
class TextureSourceBinder;
class EditTextArea : public EditWindow
{
public:
	~EditTextArea();
	EditTextArea() = delete;
	EditTextArea(TextArea* textArea, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	TextArea* m_textArea;
};