#pragma once
#include "../UIComponent.h"

class ScrollBar : public UIComponent
{
public:
	ScrollBar();
	~ScrollBar();
	static ComponentID GetTypeStatic() { return ComponentID::ScrollBar; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }

	bool Setup(const UILayout& layout, unique_ptr<UIComponent> scrollTrack, unique_ptr<UIComponent> scrollButton);
	virtual void SerializeIO(JsonOperation& operation) override;

protected:
	ScrollBar(const ScrollBar& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	UIComponent* m_scrollTrack;
	UIComponent* m_scrollButton;
};

unique_ptr<UIComponent> CreateScrollBar(const UILayout& layout,
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollButton);