#pragma once

struct ILoadData;
struct IRender;

class ImagePart
{
public:
	ImagePart(const std::wstring& filename);

	bool Load(ILoadData* load);
	void Render(IRender* render, const Vector2& origin);

	inline void SetLocalPosition(const Vector2& localPos) { m_localPosition = localPos; }
	inline void SetPosition(const Vector2& position) { m_position = position; }
	inline void SetSize(const XMUINT2& size) 
	{ 
		m_size = size; 
	}
	inline XMUINT2 GetSize() const noexcept { return m_size; }

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};
	XMUINT2 m_size{};
	Vector2 m_localPosition{};
	Vector2 m_position{};
};
