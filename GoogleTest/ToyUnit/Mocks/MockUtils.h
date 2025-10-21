#pragma once
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIComponent/UIComponentEx.h"

struct ComponentDesc
{
	ComponentDesc(const XMINT2& _p, const UILayout& _l) : position{ _p }, layout{ _l } {}
	ComponentDesc(int32_t _x, int32_t _y, const UILayout& _l) : position{ _x, _y }, layout{ _l } {}

	XMINT2 position{ 0, 0 };
	UILayout layout{};
};

namespace mock_defaults
{
	inline const XMINT2 mockPos{ 400, 300 }; //���� �����϶� component ��ġ��
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
ComponentType* CreateOneLevelComponent(UIComponent* root, const ComponentDesc& compDesc = mock_defaults::mockDesc)
{
	auto [comp, compPtr] = CreateMockComponent<ComponentType>(compDesc.layout);
	UIEx(root).AttachComponent(move(comp), compDesc.position);
	return compPtr;
}

template<typename ComponentType>
pair<ComponentType*, ComponentType*> CreateTwoLevelComponents(UIComponent* root,
	const ComponentDesc& parentDesc = mock_defaults::parentDesc,
	const ComponentDesc& childDesc = mock_defaults::childDesc)
{
	auto [parent, parentPtr] = CreateMockComponent<ComponentType>(parentDesc.layout);
	auto [child, childPtr] = CreateMockComponent<ComponentType>(childDesc.layout);

	UIEx(parent).AttachComponent(move(child), childDesc.position);
	UIEx(root).AttachComponent(move(parent), parentDesc.position);

	return { parentPtr, childPtr };
}

template <typename Func>
auto ExecuteAndUpdate(UIComponent* component, Func&& func) noexcept
{
	if constexpr (is_void_v<decltype(func())>) {
		func();
		component->UpdatePositionsManually(true);
	}
	else
	{
		auto result = func();
		component->UpdatePositionsManually(true);
		return result;
	}
}