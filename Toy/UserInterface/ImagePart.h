#pragma once

class JsonOperation;
struct ILoadData;
struct IRender;

class ImagePart
{
public:
	ImagePart();
	ImagePart(const ImagePart& other) noexcept = default;
	ImagePart(ImagePart&& other) noexcept = default;
	ImagePart(const std::wstring& filename, const Rectangle& source);
	bool operator==(const ImagePart& o) const noexcept;
	string GetType() const;

	bool Load(ILoadData* load);
	bool IsHover(int mouseX, int mouseY) const noexcept;
	void Render(IRender* render, bool selected);
	
	inline void SetPosition(const XMINT2& position) noexcept { m_position = position; }
	inline void SetDestination(const Rectangle& destination) noexcept { m_destination = destination; }

	inline const Rectangle& GetSource() const noexcept { return m_source; }
	inline XMUINT2 GetSize() const noexcept { return XMUINT2(m_source.width, m_source.height); }

	void SerializeIO(JsonOperation& operation);

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};
	
	XMINT2 m_position{};
	Rectangle m_destination{};
	Rectangle m_source{};
};
