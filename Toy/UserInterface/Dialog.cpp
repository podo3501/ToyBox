#include "pch.h"
#include "Dialog.h"

Dialog::Dialog()
{};
Dialog::~Dialog() = default;

Dialog::Dialog(const Dialog& other)
	: IRenderItem{ other }
{}

unique_ptr<IRenderItem> Dialog::Clone()
{ 
	return make_unique<Dialog>(*this);
}
