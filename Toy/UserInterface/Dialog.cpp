#include "pch.h"
#include "Dialog.h"
#include "../Utility.h"
#include "JsonOperation.h"

Dialog::Dialog()
{
};
Dialog::~Dialog() = default;

//Dialog::Dialog(const Dialog& other) :
//	UIComponent{ other }
//{
//}

//Dialog& Dialog::operator=(const Dialog& other)
//{
//	if (this == &other) return *this;
//	UIComponent::operator=(other);
//
//	return *this;
//}

bool Dialog::operator==(const UIComponent& other) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(other));

	return true;
}

//unique_ptr<UIComponent> Dialog::Clone()
//{ 
//	unique_ptr<UIComponent> clone = make_unique<Dialog>(*this);
//	clone->SetName(clone->GetName() + "_clone");
//	return clone;
//}

void Dialog::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
}
