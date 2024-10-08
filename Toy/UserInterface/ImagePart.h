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
	
	inline void SetPosition(const XMUINT2& position) { m_position = position; }
	inline void SetDestination(const Rectangle& destination) { m_destination = destination; }
	inline void SetSize(const XMUINT2& size) { m_size = size; }

	inline XMUINT2 GetSize() const noexcept { return m_size; }

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};
	XMUINT2 m_size{};
	XMUINT2 m_position{};
	Rectangle m_destination{};
	Rectangle m_source{};
};
