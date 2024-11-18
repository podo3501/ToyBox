#pragma once

enum class Origin;
class JsonOperation;

class UILayout
{
public:
	UILayout() = delete;
	~UILayout();

	UILayout(const nlohmann::json& data);
	UILayout(const Rectangle& area, const Origin& origin);
	UILayout(const UILayout& other) = default;
	UILayout& operator=(const UILayout& other);
	UILayout(UILayout&& other) noexcept = default;
	bool operator==(const UILayout& o) const noexcept;

	void Set(const Rectangle& area, const Origin& origin) noexcept;
	void Set(const Rectangle& area) noexcept;
	void Set(const Origin& origin) noexcept;

	const Rectangle& GetArea() const noexcept { return m_area; }
	Vector2 GetPosition(const Vector2& resolution) const noexcept;

	void Union(const Rectangle& area) noexcept;

	bool IsArea(const Vector2& pos) const noexcept;

	friend void to_json(nlohmann::ordered_json& j, const UILayout& data);
	friend void from_json(const nlohmann::json& j, UILayout& data);

	void SerializeIO(JsonOperation* operation);

private:
	Vector2 GetOriginPoint(Origin origin) const noexcept;

	Rectangle m_area{};
	Vector2 m_originPoint{};
	Origin m_origin;
};
