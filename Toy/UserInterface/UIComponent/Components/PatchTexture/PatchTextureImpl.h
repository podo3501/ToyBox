#pragma once

class PatchTextureBase;
class UIComponent;
class JsonOperation;
enum class DirectionType;

class PatchTextureImpl
{
public:
	~PatchTextureImpl();
	PatchTextureImpl() noexcept;
	//PatchTextureBase
	void SetIndexedSource(size_t index, const vector<Rectangle>& sources, function<vector<Rectangle>(size_t idx)> SourceSelectorFn) noexcept;
	bool FitToTextureSource(DirectionType dirType, function<void(const XMUINT2&, XMUINT2&)> AccumulateSizeFn);
	bool ChangeSize(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept;

	inline void SetOwner(PatchTextureBase* component) noexcept { m_component = component; }

private:
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(DirectionType dirType, const XMUINT2& size, const vector<XMUINT2>& sizes) noexcept;
	bool ForEachTexture(predicate<PatchTextureBase*, size_t> auto&& each);

	PatchTextureBase* m_component;
};
