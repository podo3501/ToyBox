#pragma once

enum class Origin;

class UILayout
{
public:
	UILayout();
	~UILayout();

	const Vector2& ChangeOrigin(Origin origin);
	void Set(const Rectangle& area, const Vector2& pos, Origin origin);

	const Rectangle& GetArea() const noexcept { return m_area; }
	const Vector2& GetOrigin() const noexcept { return m_originPosition; }
	const Vector2& GetPosition() const noexcept { return m_position; }

private:
	Rectangle m_area{};
	Vector2 m_position{};
	Vector2 m_originPosition{};
};
