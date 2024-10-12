#pragma once

enum class Origin;

class UILayout
{
public:
	UILayout();
	~UILayout();

	void Set(const Rectangle& area, const Vector2& pos, Origin origin) noexcept;
	void SetOrigin(Origin origin) noexcept;

	const Rectangle& GetArea() const noexcept { return m_area; }
	XMUINT2 GetPosition(const Vector2& resolution) const noexcept;
	void ChangeArea(const Rectangle& area) noexcept;

private:
	XMUINT2 GetOriginPoint(Origin origin) const noexcept;

	Rectangle m_area{};
	Vector2 m_normalizedPosition{};
	XMUINT2 m_originPoint{};
	Origin m_origin;
};
