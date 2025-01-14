#include "pch.h"
#include "Dialog.h"
#include "../../Utility.h"
#include "../JsonOperation.h"

Dialog::~Dialog() = default;
Dialog::Dialog()
{};

Dialog::Dialog(const Dialog& other) :
	UIComponent{ other }
{}

unique_ptr<UIComponent> Dialog::CreateClone() const
{
	return unique_ptr<Dialog>(new Dialog(*this));
}
