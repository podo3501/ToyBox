#include "pch.h"
#include "Editbutton.h"
#include "../Toy/UserInterface/UIComponent/UIType.h"
#include "../Toy/UserInterface/UIComponent/Components/Button.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"

EditButton::~EditButton() {};
EditButton::EditButton(Button* button, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept :
	EditWindow{ button, sourceBinder, cmdList },
    m_button{ button }
{
}

void EditButton::RenderComponent()
{
    //string state = EnumToString<InteractState>(m_button->GetState());
    //if (SelectComboItem("State", EnumToList<InteractState>(), state))
    //    m_button->SetState(StringToEnum<InteractState>(state));
}

