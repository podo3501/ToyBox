#pragma once
#include "UIComponent.h"

class Dialog : public UIComponent
{
public:
	Dialog();
	~Dialog();
	Dialog(const Dialog& other);
	
	virtual bool operator==(const UIComponent& other) const noexcept override;
	virtual string GetType() const override;
	virtual unique_ptr<UIComponent> Clone() override;

	friend void to_json(nlohmann::ordered_json& j, const Dialog& data);
	friend void from_json(const nlohmann::json& j, Dialog& data);
	
private:
	int m_test{ 3 };
};