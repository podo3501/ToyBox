#pragma once

class UILayout;
class JsonOperation;

class UITransform
{
public:
	UITransform();
	UITransform(const XMINT2& relativePos, const Vector2& ratio);
	UITransform(const XMUINT2& size, const XMINT2& relativePos);
	bool operator==(const UITransform& o) const noexcept;

	void Clear() noexcept;
	const XMINT2& GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept;
	void SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept;
	void AdjustPosition(const XMUINT2& size) noexcept;
	void SerializeIO(JsonOperation& operation);

	inline const XMINT2& GetRelativePosition() const noexcept { return m_relativePosition; }
	inline const Vector2& GetRatio() const noexcept { return m_ratio; }
	inline const XMINT2& GetAbsolutePosition() const noexcept { return m_absolutePosition; }

private:
	XMINT2 m_relativePosition{};
	Vector2 m_ratio{};
	XMINT2 m_absolutePosition{};
};
