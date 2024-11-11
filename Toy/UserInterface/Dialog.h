#pragma once
#include "UIComponent.h"

class Dialog : public UIComponent
{
public:
	Dialog();
	Dialog(const Dialog& other);
	~Dialog();

	virtual unique_ptr<UIComponent> Clone() override;
	
private:
};