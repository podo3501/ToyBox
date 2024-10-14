#pragma once

enum class Origin;

class UILayout
{
public:
	UILayout() = delete;
	~UILayout();

	UILayout(Rectangle&& area, Vector2&& normalizedPos, Origin&& origin);
	UILayout(const UILayout& layout) noexcept = default;
	UILayout(UILayout&& layout) noexcept = default;

	void Set(Rectangle&& area, Vector2&& normalizedPos, Origin&& origin) noexcept;
	void Set(Rectangle&& area) noexcept;
	void Set(Vector2&& normalPos) noexcept;
	void Set(Origin&& origin) noexcept;
	void Set(Rectangle&& area, Vector2&& normalPos) noexcept;

	const Rectangle& GetArea() const noexcept { return m_area; }
	XMUINT2 GetPosition(const Vector2& resolution) const noexcept;

private:
	XMUINT2 GetOriginPoint(Origin origin) const noexcept;

	Rectangle m_area{};
	Vector2 m_normalizedPosition{};
	XMUINT2 m_originPoint{};
	Origin m_origin;
};
