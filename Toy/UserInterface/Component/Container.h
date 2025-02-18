#pragma once
#include "../UIComponent.h"

class Container : public UIComponent
{
public:
	~Container();
	Container() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::Container; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	//bool LoadResources(ITextureLoad* load);
	bool Setup(const UILayout& layout, map<ButtonState, unique_ptr<UIComponent>>&& imgGridList) noexcept;

protected:
	Container(const Container& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	bool ImplementActiveUpdate(const XMINT2& absolutePosition) noexcept override;

private:
	void ReloadDatas() noexcept;
	void SetState(ButtonState state) noexcept;
	void AttachComponent(ButtonState btnState, unique_ptr<UIComponent>&& component) noexcept;

	map<ButtonState, UIComponent*> m_images;
	optional<ButtonState> m_state;
};

unique_ptr<Container> CreateContainer(const UILayout& layout, map<ButtonState, unique_ptr<UIComponent>>&& imgGridList);