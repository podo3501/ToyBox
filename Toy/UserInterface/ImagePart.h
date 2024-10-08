#pragma once

struct ILoadData;
struct IRender;

class ImagePart
{
public:
	ImagePart(const std::wstring& filename);

	bool Load(ILoadData* load);
	void MakeLocalDestination(const Vector2& origin);
	bool IsOver(int mouseX, int mouseY) const noexcept;
	void Render(IRender* render) const;
	
	inline void SetPosition(const Vector2& position) { m_position = position; }
	inline void SetLocalPosition(const Vector2& localPos) { m_localPosition = localPos; }
	inline void SetSize(const XMUINT2& size) { m_size = size; }

	inline XMUINT2 GetSize() const noexcept { return m_size; }

private:
	

	size_t m_index{ 0 };
	std::wstring m_filename{};
	XMUINT2 m_size{};
	Vector2 m_localPosition{};
	Vector2 m_position{};
	Rectangle m_localDestination{};
};
