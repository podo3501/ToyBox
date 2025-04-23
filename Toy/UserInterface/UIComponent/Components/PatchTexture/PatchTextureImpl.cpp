#include "pch.h"
#include "PatchTextureImpl.h"
#include "PatchTexture.h"
#include "PatchTextureLite/PatchTextureLite.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

PatchTextureImpl::~PatchTextureImpl() = default;
PatchTextureImpl::PatchTextureImpl() noexcept :
	m_component{ nullptr }
{}
//?!? 이걸 조금 변형하면 utility에 foreach를 넣고 리스트들을 돌면서 특정 component로 형변환 해서 넘겨주는 거 만들수도 있겠다.
template<typename T, predicate<T*, size_t> Func>
bool PatchTextureImpl::ForEach(Func&& each)
{
	const auto& components = m_component->GetChildComponents();
	size_t size = components.size();
	ReturnIfFalse(size >= 3);

	for (size_t n : views::iota(0u, size))
		if (!each(static_cast<T*>(components[n]), n)) return false;

	return true;
}
//?!? 이건 PatchTextureLite로 옮기자. 그러면 이 파일은 지워진다.
void PatchTextureImpl::SetIndexedSource( size_t index, const vector<Rectangle>& sources,
	function<vector<Rectangle>(size_t idx)> SourceSelectorFn) noexcept
{
	ForEach<PatchTextureLite>([index, &sources, &SourceSelectorFn](PatchTextureLite* tex, size_t idx) {
		tex->SetIndexedSource(index, SourceSelectorFn(idx));
		return true;
		});
}
