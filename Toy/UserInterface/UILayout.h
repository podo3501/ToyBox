#pragma once

enum class Origin;

class UILayout
{
public:
	UILayout() = delete;
	~UILayout();

	UILayout(const nlohmann::json& data);
	UILayout(const Rectangle& area, const Vector2& normalizedPos, const Origin& origin);
	UILayout(const UILayout& other) noexcept = default;
	UILayout(UILayout&& other) noexcept = default;
	UILayout& operator=(const UILayout& other) noexcept = default;

	void Set(const Rectangle& area, const Vector2& normalizedPos, const Origin& origin) noexcept;
	void Set(const Rectangle& area) noexcept;
	void Set(const Vector2& normalPos) noexcept;
	void Set(const Origin& origin) noexcept;
	void Set(const Rectangle& area, const Vector2& normalPos) noexcept;

	const Rectangle& GetArea() const noexcept { return m_area; }
	Vector2 GetPosition(const Vector2& resolution) const noexcept;

	void Union(const Rectangle& area) noexcept;

	bool IsArea(const Vector2& pos) const noexcept;

private:
	Vector2 GetOriginPoint(Origin origin) const noexcept;

	Rectangle m_area{};
	Vector2 m_normalizedPosition{};
	Vector2 m_originPoint{};
	Origin m_origin;
};
