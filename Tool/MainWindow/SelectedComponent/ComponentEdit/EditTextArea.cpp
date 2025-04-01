#include "pch.h"
#include "EditTextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/Command/UICommandList.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"

EditTextArea::~EditTextArea() = default;
EditTextArea::EditTextArea(TextArea* textArea, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
	EditWindow{ textArea, sourceBinder, cmdList },
	m_textArea{ textArea }
{}

void EditTextArea::RenderComponent()
{
	wstring text = m_textArea->GetText();
	if(EditText("Text", text))
		GetUICommandList()->SetText(m_textArea, text);
}