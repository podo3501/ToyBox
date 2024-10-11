#pragma once

enum class Origin;

class UILayout
{
public:
	UILayout();
	~UILayout();

	void Set(const Rectangle& area, const Vector2& pos, Origin origin);
	void SetOrigin(Origin origin);

	const Rectangle& GetArea() const noexcept { return m_area; }
	XMUINT2 GetPosition(const Vector2& resolution) const noexcept;

private:
	XMUINT2 GetOriginPoint(Origin origin) noexcept;

	Rectangle m_area{};
	Vector2 m_normalizedPosition{};
	XMUINT2 m_originPoint{};
};
