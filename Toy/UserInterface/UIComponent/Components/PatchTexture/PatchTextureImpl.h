#pragma once

class PatchTexture;
class UIComponent;
enum class DirectionType;
class PatchTextureImpl
{
public:
	~PatchTextureImpl();
	PatchTextureImpl() noexcept;
	//PatchTexture
	void SetIndexedSource(size_t index, const vector<Rectangle>& sources, function<vector<Rectangle>(size_t idx)> SourceSelectorFn) noexcept;
	//bool FitToTextureSource(DirectionType dirType, function<void(const XMUINT2&, XMUINT2&)> AccumulateSizeFn);
	//bool ChangeSize(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept;

	inline void SetOwner(PatchTexture* component) noexcept { m_component = component; }

private:
	//bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	//bool ApplyPositions(DirectionType dirType, const XMUINT2& size, const vector<XMUINT2>& sizes) noexcept;
	template<typename T, predicate<T*, size_t> Func>
	bool ForEach(Func&& each);

	PatchTexture* m_component;
};

inline vector<Rectangle> GetTripleAt(const vector<Rectangle>& src, size_t idx) noexcept
{
	size_t base = idx * 3;
	return { src[base + 0], src[base + 1], src[base + 2] };
}