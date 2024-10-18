#pragma once

struct ILoadData;
struct IRender;

class ImagePart
{
public:
	ImagePart() = delete;
	ImagePart(const std::wstring& filename, const Rectangle& source);

	bool Load(ILoadData* load);
	bool IsHover(int mouseX, int mouseY) const noexcept;
	void Render(IRender* render);
	
	inline void SetPosition(const Vector2& position) noexcept { m_position = position; }
	inline void SetDestination(const Rectangle& destination) noexcept { m_destination = destination; }

	inline const Rectangle& GetSource() const noexcept { return m_source; }
	inline XMUINT2 GetSize() const noexcept { return XMUINT2(m_source.width, m_source.height); }

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};
	
	Vector2 m_position{};
	Rectangle m_destination{};
	Rectangle m_source{};
};
