#pragma once
#include "UIComponent.h"

class JsonOperation;

class Dialog : public UIComponent
{
protected:
	Dialog(const Dialog& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	Dialog();
	~Dialog();
	
private:
};