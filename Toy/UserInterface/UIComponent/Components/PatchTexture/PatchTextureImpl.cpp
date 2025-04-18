#include "pch.h"
#include "PatchTextureImpl.h"
#include "../Include/IRenderer.h"
#include "../PatchTexture/PatchTexture1.h"
#include "Utility.h"
#include "../../UIUtility.h"
#include "../../../JsonOperation/JsonOperation.h"

PatchTextureImpl::~PatchTextureImpl() = default;
PatchTextureImpl::PatchTextureImpl() noexcept :
	m_component{ nullptr },
	m_dirType{ DirectionType::Horizontal }
{}

PatchTextureImpl::PatchTextureImpl(const PatchTextureImpl& o) noexcept :
	m_dirType{ o.m_dirType }
{}

bool PatchTextureImpl::operator==(const PatchTextureImpl& rhs) const noexcept
{
	return(tie(m_dirType) == tie(rhs.m_dirType));
}

bool PatchTextureImpl::ForEachTexture(predicate<PatchTextureBase*, size_t> auto&& each)
{
	const auto& components = m_component->GetChildComponents();
	size_t size = components.size();
	ReturnIfFalse(size >= 3);

	for (size_t n : views::iota(0u, size))
		if (!each(static_cast<PatchTextureBase*>(components[n]), n)) return false;

	return true;
}

bool PatchTextureImpl::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachTexture([this, &sizes](PatchTextureBase* tex, size_t index) {
		return tex->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTextureImpl::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(m_dirType, sizes);
	ReturnIfFalse(ForEachTexture([this, &size, &positions](PatchTextureBase*, size_t index) {
		return m_component->ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTextureImpl::ChangeSize(const XMUINT2& size, const vector<UIComponent*>& components) noexcept
{
	//여기서  자식들의 기본 사이즈가 없어서 stretch가 되지 않았다.
	vector<XMUINT2> sizes = StretchSize(m_dirType, size, components);
	ReturnIfFalse(ApplyStretchSize(sizes));
	ReturnIfFalse(ApplyPositions(size, sizes));
	m_component->ApplySize(size);

	return true;
}

void PatchTextureImpl::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	ForEachTexture([index, &sources](PatchTextureBase* tex, size_t idx) {
		tex->SetIndexedSource(index, { sources[idx] });
		return true;
		});
}

bool PatchTextureImpl::FitToTextureSource() noexcept
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachTexture([&totalSize, &sizes, this](PatchTextureBase* tex, size_t) {
		ReturnIfFalse(tex->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(tex->GetArea());
		sizes.emplace_back(size);
		switch (m_dirType) {
		case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
		case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
		}
		return true;
		});
	ReturnIfFalse(result); //result 변수를 써서 ReturnIfFalse를 한 이유는 람다에 디버깅(브레이크 포인터)이 안되기 때문이다.
	ReturnIfFalse(ApplyPositions(totalSize, sizes));

	m_component->SetSize(totalSize);
	return true;
}

void PatchTextureImpl::SerializeIO(JsonOperation& operation)
{
	operation.Process("DirectionType", m_dirType);
}