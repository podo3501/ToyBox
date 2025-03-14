#include "pch.h"
#include "EditTextArea.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"

EditTextArea::~EditTextArea() = default;
EditTextArea::EditTextArea(TextArea* textArea, CommandList* cmdList) noexcept :
	EditWindow{ textArea, cmdList },
	m_textArea{ textArea }
{}

void EditTextArea::RenderComponent()
{
	wstring text = m_textArea->GetText();
	if(EditText("Text", text))
		GetCommandList()->SetText(m_textArea, text);
}