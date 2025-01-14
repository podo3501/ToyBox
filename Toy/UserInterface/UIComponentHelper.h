#pragma once

class UIComponent;

bool AddComponentFromScreenPos(
	UIComponent* addable, 
	unique_ptr<UIComponent>&& component, 
	const XMINT2& pos) noexcept;

Rectangle GetRectangle(const UIComponent* component) noexcept;