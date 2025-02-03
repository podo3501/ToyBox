#include "pch.h"
#include "EditTextArea.h"
#include "../Toy/UserInterface/Component/TextArea.h"

EditTextArea::~EditTextArea() = default;
EditTextArea::EditTextArea(TextArea* textArea, CommandList* cmdList) noexcept :
	EditWindow{ textArea, cmdList },
	m_textArea{ textArea }
{}

void EditTextArea::RenderComponent()
{

}