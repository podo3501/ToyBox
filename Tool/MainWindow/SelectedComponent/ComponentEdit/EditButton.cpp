#include "pch.h"
#include "Editbutton.h"
#include "../Toy/UserInterface/UIComponent/UIType.h"
#include "../Toy/UserInterface/UIComponent/Components/Button.h"
#include "../Toy/Utils/CommonUtil.h"
#include "EditUtility/EditUtility.h"

EditButton::~EditButton() {};
EditButton::EditButton(Button* button, UICommandList* cmdList) noexcept :
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

