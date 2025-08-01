#include "pch.h"
#include "Dialog.h"
#include "Utils/CommonUtil.h"
#include "../../JsonOperation/JsonOperation.h"

Dialog::~Dialog() = default;
Dialog::Dialog()
{};

Dialog::Dialog(const Dialog& other) :
	UIComponent{ other }
{}

unique_ptr<UIComponent> Dialog::CreateClone() const
{
	return unique_ptr<Dialog>(new Dialog(*this)); //��������ڸ� protected�� ������� make_unique��� new�� ����ؾ��Ѵ�.
}
