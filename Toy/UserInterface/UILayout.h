#pragma once

enum class Origin;
class JsonOperation;

class UILayout
{
public:
	UILayout();
	~UILayout();

	UILayout(const nlohmann::json& data);
	UILayout(const Rectangle& area, const Origin& origin);
	UILayout(const UILayout& other) = default;
	UILayout& operator=(const UILayout& other);
	UILayout(UILayout&& other) noexcept = default;
	bool operator==(const UILayout& o) const noexcept;

	string GetType() const;

	void Set(const Rectangle& area, const Origin& origin) noexcept;
	void Set(const Rectangle& area) noexcept;
	void Set(const Origin& origin) noexcept;

	const Rectangle& GetArea() const noexcept { return m_area; }
	XMINT2 GetPosition(const Vector2& resolution = {}) const noexcept;

	void Union(const Rectangle& area) noexcept;

	bool IsArea(const XMINT2& pos) const noexcept;

	void SerializeIO(JsonOperation& operation);

private:
	XMINT2 GetOriginPoint(Origin origin) const noexcept;

	Rectangle m_area{};
	XMINT2 m_originPoint{};
	Origin m_origin;
};
