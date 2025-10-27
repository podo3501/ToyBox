#pragma once

struct ITextureRender;
class UIComponent;
class PatchTextureCoord
{
public:
	~PatchTextureCoord();
	PatchTextureCoord() noexcept;
	PatchTextureCoord(const PatchTextureCoord& o) noexcept;
	bool operator==(const PatchTextureCoord& rhs) const noexcept;

	void SetOwner(UIComponent* component) noexcept { m_component = component; }
	void Render(ITextureRender* render) const;
	void SetIndexedSource(size_t index, const Rectangle& source) noexcept;
	bool FitToTextureSource() noexcept;
	optional<size_t> GetIndex() const noexcept { return  m_index; }
	const Rectangle& GetSource() const noexcept { return  m_source; }

private:
	UIComponent* m_component;
	optional<size_t> m_index; //�ؽ��� �ε���. 0���� �ε����� ����ϱ� ������ optional
	Rectangle m_source{};
};

XMUINT2 GetSourceSize(const PatchTextureCoord& coord) noexcept;
