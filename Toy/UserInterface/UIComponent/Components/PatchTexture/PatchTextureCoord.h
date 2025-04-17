#pragma once

struct ITextureRender;
class UIComponent;
class PatchTextureCoord
{
public:
	~PatchTextureCoord();
	PatchTextureCoord() = delete;
	explicit PatchTextureCoord(UIComponent* component) noexcept;
	PatchTextureCoord(const PatchTextureCoord& o) noexcept;

	void Render(ITextureRender* render) const;
	void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept;
	bool FitToTextureSource() noexcept;
	optional<size_t> GetIndex() const noexcept { return  m_index; }
	const Rectangle& GetSource() const noexcept { return  m_source; }

private:
	UIComponent* m_component;
	optional<size_t> m_index; //텍스쳐 인덱스. 0값도 인덱스로 사용하기 때문에 optional
	Rectangle m_source{};
};

XMUINT2 GetSourceSize(const PatchTextureCoord& coord) noexcept;
