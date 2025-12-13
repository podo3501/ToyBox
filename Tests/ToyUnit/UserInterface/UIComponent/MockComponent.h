#pragma once
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

class UIComponentStub : public UIComponent
{
public:
	virtual ComponentID GetTypeID() const noexcept { return UIComponent::GetTypeStatic(); }

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override { return nullptr; }
};

class MockComponent : public UIComponentStub
{
public:
	bool Setup() noexcept; //CreateComponent<T> 할때 필요한 함수.

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;
};

struct ComponentDesc
{
	ComponentDesc(const XMINT2& _p, const UILayout& _l) : position{ _p }, layout{ _l } {}
	ComponentDesc(int32_t _x, int32_t _y, const UILayout& _l) : position{ _x, _y }, layout{ _l } {}

	XMINT2 position{ 0, 0 };
	UILayout layout{};
};

namespace mock_defaults
{
	inline const XMINT2 mockPos{ 400, 300 }; //단일 생성일때 component 위치값
	inline const XMINT2 parentPos{ 400, 300 };
	inline const XMINT2 childPos{ 50, 50 };
	inline const UILayout defaultLayout{ { 100, 100 }, Origin::LeftTop };

	inline const ComponentDesc mockDesc{ mockPos, defaultLayout };
	inline const ComponentDesc parentDesc{ parentPos, defaultLayout };
	inline const ComponentDesc childDesc{ childPos, defaultLayout };
}

template<typename ComponetType>
pair<unique_ptr<ComponetType>, ComponetType*> CreateMockComponent(const UILayout& layout = mock_defaults::defaultLayout)
{
	auto comp = make_unique<ComponetType>();
	comp->SetLayout(layout);

	return make_pair(move(comp), comp.get());
}

template<typename ComponentType>
ComponentType* AttachMockComponent(UIComponent* root, const ComponentDesc& compDesc = mock_defaults::mockDesc)
{
	auto [comp, compPtr] = CreateMockComponent<ComponentType>(compDesc.layout);
	UITraverser::AttachComponent(root, move(comp), compDesc.position);
	return compPtr;
}

template<typename ComponentType>
ComponentType* AttachMockComponentDirect(UIComponent* root, const ComponentDesc& compDesc = mock_defaults::mockDesc)
{
	auto [comp, compPtr] = CreateMockComponent<ComponentType>(compDesc.layout);
	root->AttachComponent(move(comp), compDesc.position);
	return compPtr;
}

