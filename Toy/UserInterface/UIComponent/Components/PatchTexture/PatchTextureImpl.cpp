#include "pch.h"
#include "PatchTextureImpl.h"
#include "PatchTexture.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

PatchTextureImpl::~PatchTextureImpl() = default;
PatchTextureImpl::PatchTextureImpl() noexcept :
	m_component{ nullptr }
{}

bool PatchTextureImpl::ForEachTexture(predicate<PatchTexture*, size_t> auto&& each)
{
	const auto& components = m_component->GetChildComponents();
	size_t size = components.size();
	ReturnIfFalse(size >= 3);

	for (size_t n : views::iota(0u, size))
		if (!each(static_cast<PatchTexture*>(components[n]), n)) return false;

	return true;
}

bool PatchTextureImpl::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachTexture([this, &sizes](PatchTexture* tex, size_t index) {
		return tex->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTextureImpl::ApplyPositions(DirectionType dirType, const XMUINT2& size, const vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(dirType, sizes);
	ReturnIfFalse(ForEachTexture([this, &size, &positions](PatchTexture*, size_t index) {
		return m_component->ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTextureImpl::ChangeSize(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept
{
	const vector<XMUINT2>& sizes = StretchSize(dirType, size, components);
	ReturnIfFalse(ApplyStretchSize(sizes));
	ReturnIfFalse(ApplyPositions(dirType, size, sizes));
	m_component->ApplySize(size);

	return true;
}

void PatchTextureImpl::SetIndexedSource( size_t index, const vector<Rectangle>& sources,
	function<vector<Rectangle>(size_t idx)> SourceSelectorFn) noexcept
{
	ForEachTexture([index, &sources, &SourceSelectorFn](PatchTexture* tex, size_t idx) {
		tex->SetIndexedSource(index, SourceSelectorFn(idx));
		return true;
		});
}

bool PatchTextureImpl::FitToTextureSource(DirectionType dirType, function<void(const XMUINT2&, XMUINT2&)> AccumulateSizeFn)
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachTexture([&totalSize, &sizes, AccumulateSizeFn](PatchTexture* tex, size_t) {
		ReturnIfFalse(tex->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(tex->GetArea());
		sizes.emplace_back(size);
		AccumulateSizeFn(size, totalSize);
		return true;
		});
	ReturnIfFalse(result); //result 변수를 써서 ReturnIfFalse를 한 이유는 람다에 디버깅(브레이크 포인터)이 안되기 때문이다.
	ReturnIfFalse(ApplyPositions(dirType, totalSize, sizes));

	m_component->SetSize(totalSize);
	return true;
}