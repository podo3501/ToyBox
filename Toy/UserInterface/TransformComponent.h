#pragma once

class UILayout;
class UIComponent;
class JsonOperation;

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(unique_ptr<UIComponent> comp, const XMINT2& relativePos, const Vector2& ratio);
	TransformComponent(unique_ptr<UIComponent> comp, const XMUINT2& size, const XMINT2& relativePos);
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator=(const TransformComponent&) = delete;
	TransformComponent(TransformComponent&& o) noexcept;
	bool operator==(const TransformComponent& o) const noexcept;

	const XMINT2& GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept;
	const XMINT2& GetRelativePosition() const noexcept { return relativePosition; }
	void SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept;
	const Vector2& GetRatio() const noexcept { return ratio; }
	void AdjustPosition(const XMUINT2& size) noexcept;
	void SerializeIO(JsonOperation& operation);

	unique_ptr<UIComponent> component;
	XMINT2 absolutePosition{};

private:
	XMINT2 relativePosition{};
	Vector2 ratio{};
};
