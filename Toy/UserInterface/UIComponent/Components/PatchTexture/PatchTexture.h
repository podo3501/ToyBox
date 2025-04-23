#pragma once
#include "../../UIComponent.h"

struct ITextureController;
struct TextureSourceInfo;

class PatchTexture : public UIComponent
{
public:
	~PatchTexture();
	virtual bool FitToTextureSource() noexcept;
	virtual UIComponent* GetCenterComponent() noexcept;
	inline optional<TextureSlice> GetTextureSlice() const noexcept { return m_texSlice; }

protected:
	PatchTexture();
	PatchTexture(TextureSlice texSlice) noexcept;
	PatchTexture(const PatchTexture& other) noexcept;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept override;
	virtual vector<Rectangle> GetChildSourceList() const noexcept { return {}; }
	inline void SetTextureSlice(TextureSlice texSlice) noexcept { m_texSlice = texSlice; }
	
private:
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(DirectionType dirType, const XMUINT2& size, const vector<XMUINT2>& sizes) noexcept;
	bool ApplySizeAndPosition(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept;
	bool ForEach(predicate<PatchTexture*, size_t> auto&& Each);

	optional<TextureSlice> m_texSlice{ nullopt };

	friend class PatchTextureImpl;
};

template<typename T, typename Func>
vector<Rectangle> GetSourceList(const vector<UIComponent*>& components, Func GetterFn) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [GetterFn](auto component) {
		T* tex = ComponentCast<T*>(component);
		return (tex->*GetterFn)();
		});
	return srcList;
}