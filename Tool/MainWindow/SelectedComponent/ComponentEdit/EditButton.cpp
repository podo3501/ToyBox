#include "pch.h"
#include "Editbutton.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/Utility.h"

EditButton::~EditButton() {};
EditButton::EditButton(Button* button, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ button, cmdList },
    m_button{ button }
{
}

void EditButton::UpdateComponent()
{

}

bool SelectComboItem(const string& label, const vector<string>& items, string& select)
{
    string prev = select;

    ReturnIfFalse(ImGui::BeginCombo(label.c_str(), select.c_str(), ImGuiComboFlags_HeightSmall));
    for (const auto& item : items)
    {
        bool is_selected = (item == select);
        if (ImGui::Selectable(item.c_str(), is_selected))
            select = item;

        if (is_selected)
           ImGui::SetItemDefaultFocus(); //Ű����� ���Ʒ� ���� �˻��Ҷ� ������ �����ۿ��� �����Ѵ�.
    }

    ImGui::EndCombo();

    return prev != select;
}

void EditButton::RenderComponent()
{
    string state = EnumToString<ButtonState>(m_button->GetState());
    if (SelectComboItem("State", EnumToList<ButtonState>(), state))
        m_button->SetState(StringToEnum<ButtonState>(state));
}

