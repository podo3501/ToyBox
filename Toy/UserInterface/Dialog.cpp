#include "pch.h"
#include "Dialog.h"
#include "../Utility.h"
#include "JsonHelper.h"
#include "JsonOperation.h"

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

unique_ptr<UIComponent> Dialog::Clone()
{ 
	return make_unique<Dialog>(*this);
}

void to_json(nlohmann::ordered_json& j, const Dialog& data)
{
	to_json(j, static_cast<const UIComponent&>(data));
}

void from_json(const nlohmann::json& j, Dialog& data)
{
	from_json(j, static_cast<UIComponent&>(data));
}

void Dialog::SerializeIO(JsonOperation* operation)
{
	UIComponent::SerializeIO(operation);
}
