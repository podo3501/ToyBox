#include "pch.h"
#include "EditListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "EditUtility/EditUtility.h"
#include "../Toy/Utility.h"

static vector<string> GenerateNumberStrings(int numCount) noexcept
{ 
	vector<string> v(numCount);
	ranges::generate(v, [i = 0]() mutable { return to_string(i++); });
	return v; 
}

EditListArea::~EditListArea() = default;
EditListArea::EditListArea(ListArea* listArea, UICommandList* cmdList) noexcept :
	EditWindow{ listArea, cmdList },
	m_listArea{ listArea }
{
		m_dummyCountCombo = make_unique<EditCombo>("Dummies", GenerateNumberStrings(31));
		m_dummyCountCombo->SelectItem(0);
}

void EditListArea::RenderComponent()
{
	if (!m_dummyCountCombo) return;

	m_dummyCountCombo->Render([this](const string& key) {
		m_listArea->ClearContainers();
		for (auto n : views::iota(0, stoi(key)))
			m_listArea->PrepareContainer();
		});
}