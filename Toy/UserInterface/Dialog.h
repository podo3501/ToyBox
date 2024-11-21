#pragma once
#include "UIComponent.h"

class JsonOperation;

class Dialog : public UIComponent
{
public:
	Dialog();
	~Dialog();
	Dialog(const Dialog& other);
	Dialog& operator=(const Dialog& other);
	
	virtual bool operator==(const UIComponent& other) const noexcept override;
	virtual unique_ptr<UIComponent> Clone() override;

	virtual void SerializeIO(JsonOperation* operation) override;
	
private:
};