#pragma once

class UILayout;
class UIComponent;
class JsonOperation;

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(unique_ptr<UIComponent> comp, const XMINT2& relativePos);
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator=(const TransformComponent&) = delete;
	TransformComponent(TransformComponent&& o) noexcept;
	bool operator==(const TransformComponent& o) const noexcept;

	XMINT2 GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept;
	void SerializeIO(JsonOperation& operation);

	unique_ptr<UIComponent> component;
	XMINT2 relativePosition{};
	XMINT2 absolutePosition{};
};
