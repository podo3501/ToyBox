#pragma once

class UILayout;
class UIComponent;
class JsonOperation;

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(const XMINT2& relativePos, const Vector2& ratio);
	TransformComponent(const XMUINT2& size, const XMINT2& relativePos);
	bool operator==(const TransformComponent& o) const noexcept;

	const XMINT2& GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept;
	const XMINT2& GetRelativePosition() const noexcept { return relativePosition; }
	void SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept;
	const Vector2& GetRatio() const noexcept { return ratio; }
	void AdjustPosition(const XMUINT2& size) noexcept;
	void SerializeIO(JsonOperation& operation);

	XMINT2 absolutePosition{};

private:
	XMINT2 relativePosition{};
	Vector2 ratio{};
};
