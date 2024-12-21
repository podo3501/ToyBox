#pragma once

enum class Origin;
class JsonOperation;

class UILayout
{
public:
	UILayout();
	~UILayout();

	UILayout(const XMUINT2& size, const Origin& origin);
	UILayout(const UILayout& other) = default;
	UILayout& operator=(const UILayout& other);
	UILayout(UILayout&& other) noexcept = default;
	bool operator==(const UILayout& o) const noexcept;

	string GetType() const;

	void Set(const XMUINT2& size, const Origin& origin) noexcept;
	void Set(const XMUINT2& size) noexcept;
	void Set(const Origin& origin) noexcept;

	const XMUINT2& GetSize() const noexcept { return m_size; }
	XMINT2 GetPosition(const Vector2& position = {}) const noexcept;

	bool IsArea(const XMINT2& pos) const noexcept;

	void SerializeIO(JsonOperation& operation);

private:
	XMINT2 GetOriginPoint(Origin origin) const noexcept;

	XMUINT2 m_size{};
	XMINT2 m_originPoint{};
	Origin m_origin;
};
