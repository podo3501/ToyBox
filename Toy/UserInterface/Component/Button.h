#pragma once
#include "../UIComponent.h"

class Button : public UIComponent
{
public:
	~Button();
	Button();

	static ComponentID GetTypeStatic() { return ComponentID::Button; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent> container) noexcept;

protected:
	Button(const Button& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	void ReloadDatas() noexcept;

	UIComponent* m_container;
};

unique_ptr<Button> CreateButton(const UILayout& layout, unique_ptr<UIComponent> container);