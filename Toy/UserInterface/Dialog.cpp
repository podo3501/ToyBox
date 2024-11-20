#include "pch.h"
#include "Dialog.h"
#include "../Utility.h"
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

void Dialog::SerializeIO(JsonOperation* operation)
{
	UIComponent::SerializeIO(operation);
}
