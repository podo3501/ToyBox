#include "pch.h"
#include "Dialog.h"
#include "../Utility.h"
#include "JsonHelper.h"

Dialog::Dialog()
{};
Dialog::~Dialog() = default;

Dialog::Dialog(const Dialog& other)
	: UIComponent{ other }
{}

bool Dialog::operator==(const UIComponent& other) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(other));

	return true;
}

string Dialog::GetType() const { return string(typeid(Dialog).name()); }

unique_ptr<UIComponent> Dialog::Clone()
{ 
	return make_unique<Dialog>(*this);
}

void to_json(nlohmann::ordered_json& j, const Dialog& data)
{
	to_json(j, static_cast<const UIComponent&>(data));
	DataToJson("Test", data.m_test, j);
}

void from_json(const nlohmann::json& j, Dialog& data)
{
	from_json(j, static_cast<UIComponent&>(data));
	DataFromJson("Test", data.m_test, j);
}
