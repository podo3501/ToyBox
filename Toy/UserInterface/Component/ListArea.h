#pragma once
#include "../UIComponent.h"

class ListArea : public UIComponent
{
public:
	~ListArea();
	ListArea() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::ListArea; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent>&& bgImage, unique_ptr<UIComponent>&& container) noexcept;
	UIComponent* GetPrototypeContainer() noexcept { return m_prototypeContainer; }
	UIComponent* PrepareContainer();

protected:
	ListArea(const ListArea& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	void ReloadDatas() noexcept;

	UIComponent* m_bgImage;
	UIComponent* m_prototypeContainer;
};

unique_ptr<ListArea> CreateListArea(const UILayout& layout,
	unique_ptr<UIComponent>&& bgImage,
	unique_ptr<UIComponent>&& container);