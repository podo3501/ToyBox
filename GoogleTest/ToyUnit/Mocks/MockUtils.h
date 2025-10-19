#pragma once
#include "Toy/UserInterface/UIComponent/UILayout.h"

template<typename T>
static pair<unique_ptr<T>, T*> CreateMockComponent(optional<UILayout> layout = nullopt)
{
	auto comp = make_unique<T>();

	UILayout defaultLayout{ { 10, 10 }, Origin::LeftTop };
	comp->SetLayout(layout.value_or(defaultLayout));

	return make_pair(move(comp), comp.get());
}