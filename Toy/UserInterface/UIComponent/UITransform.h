#pragma once

class UILayout;
class JsonOperation;

class UITransform
{
public:
	UITransform();
	bool operator==(const UITransform& o) const noexcept;

	void Clear() noexcept;
	XMINT2 GetUpdatedPosition(const UILayout& layout, const XMINT2& parentPos) noexcept;
	void SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept;
	void AdjustPosition(const XMUINT2& size) noexcept;
	void SerializeIO(JsonOperation& operation);

	inline const Vector2& GetRatio() const noexcept { return m_ratio; }
	inline const XMINT2& GetRelativePosition() const noexcept { return m_relativePosition; }
	inline const XMINT2& GetAbsolutePosition() const noexcept { return m_absolutePosition; }

private:
	Vector2 m_ratio{};
	XMINT2 m_relativePosition{};
	XMINT2 m_absolutePosition{};
};
