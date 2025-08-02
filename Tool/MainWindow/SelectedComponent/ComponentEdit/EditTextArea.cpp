#include "pch.h"
#include "EditTextArea.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/Command/UICommandList/UICommandList.h"
#include "EditUtility/EditUtility.h"

EditTextArea::~EditTextArea() = default;
EditTextArea::EditTextArea(TextArea* textArea, UICommandList* cmdList) noexcept :
	EditWindow{ textArea, cmdList },
	m_textArea{ textArea }
{}

void EditTextArea::RenderComponent()
{
	wstring text = m_textArea->GetText();
	if(EditText("Text", text))
		GetUICommandList()->SetText(m_textArea, text);
}