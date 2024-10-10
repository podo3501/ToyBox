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
	const XMUINT2& GetOrigin() const noexcept { return m_originPosition; }
	const Vector2& GetPosition() const noexcept { return m_position; }

private:
	XMUINT2 GetOriginPosition(Origin origin) noexcept;

	Rectangle m_area{};
	Vector2 m_position{};
	XMUINT2 m_originPosition{};
};
