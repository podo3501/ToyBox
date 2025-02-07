#pragma once
#include "../UIComponent.h"

class ListArea : public UIComponent
{
public:
	~ListArea();
	ListArea() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::ListArea; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	bool Setup(const UILayout& layout, const ImageSource& source) noexcept;

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
};

unique_ptr<ListArea> CreateListArea(const UILayout& layout, const ImageSource& source);