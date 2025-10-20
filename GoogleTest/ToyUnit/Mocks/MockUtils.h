#pragma once
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

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
static pair<unique_ptr<ComponetType>, ComponetType*> CreateMockComponent(const optionalRef<UILayout>& layout = nullopt)
{
	const auto& currLayout = layout ? layout->get() : mock_defaults::defaultLayout;

	auto comp = make_unique<ComponetType>();
	comp->SetLayout(currLayout);

	return make_pair(move(comp), comp.get());
}

template<typename ComponentType>
ComponentType* CreateOneLevelComponent(UIComponent* root, const optionalRef<ComponentDesc>& compDesc = nullopt)
{
	const auto& currCompDesc = compDesc ? compDesc->get() : mock_defaults::mockDesc;

	auto [comp, compPtr] = CreateMockComponent<ComponentType>(currCompDesc.layout);
	UIEx(root).AttachComponent(move(comp), currCompDesc.position);
	return compPtr;
}

template<typename ComponentType>
pair<ComponentType*, ComponentType*> CreateTwoLevelComponents(UIComponent* root,
	const optionalRef<ComponentDesc>& parentDesc = nullopt,
	const optionalRef<ComponentDesc>& childDesc = nullopt)
{
	const auto& currParentDesc = parentDesc ? parentDesc->get() : mock_defaults::parentDesc;
	const auto& currChildDesc = childDesc ? childDesc->get() : mock_defaults::childDesc;

	auto [parent, parentPtr] = CreateMockComponent<ComponentType>(currParentDesc.layout);
	auto [child, childPtr] = CreateMockComponent<ComponentType>(currChildDesc.layout);

	UIEx(parent).AttachComponent(move(child), currChildDesc.position);
	UIEx(root).AttachComponent(move(parent), currParentDesc.position);

	return { parentPtr, childPtr };
}