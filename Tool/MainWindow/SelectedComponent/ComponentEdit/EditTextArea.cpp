#include "pch.h"
#include "EditTextArea.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/Utility.h"

EditTextArea::~EditTextArea() = default;
EditTextArea::EditTextArea(TextArea* textArea, CommandList* cmdList) noexcept :
	EditWindow{ textArea, cmdList },
	m_textArea{ textArea }
{}

void EditTextArea::RenderComponent()
{
	char textBuffer[128] = "";
	WStringToChar(m_textArea->GetText(), textBuffer);
	ImGui::InputText("Text", textBuffer, IM_ARRAYSIZE(textBuffer));
	if (ImGui::IsItemDeactivatedAfterEdit())
		GetCommandList()->SetText(m_textArea, StringToWString(textBuffer));
}