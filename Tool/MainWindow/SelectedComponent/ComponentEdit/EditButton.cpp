#include "pch.h"
#include "Editbutton.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"

EditButton::~EditButton() {};
EditButton::EditButton(Button* button, CommandList* cmdList) noexcept :
	EditWindow{ button, cmdList },
    m_button{ button }
{
}

void EditButton::RenderComponent()
{
    //string state = EnumToString<InteractState>(m_button->GetState());
    //if (SelectComboItem("State", EnumToList<InteractState>(), state))
    //    m_button->SetState(StringToEnum<InteractState>(state));
}

