#pragma once
#include "IRenderItem.h"

class Dialog : public IRenderItem
{
public:
	Dialog();
	Dialog(const Dialog& other);
	~Dialog();

	virtual unique_ptr<IRenderItem> Clone() override;
	
private:
};