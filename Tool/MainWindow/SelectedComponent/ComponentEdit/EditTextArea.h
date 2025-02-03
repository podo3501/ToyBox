#pragma once
#include "../EditWindow.h"

class TextArea;
class EditTextArea : public EditWindow
{
public:
	~EditTextArea();
	EditTextArea() = delete;
	EditTextArea(TextArea* textArea, CommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	TextArea* m_textArea;
};