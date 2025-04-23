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
//?!? �̰� ���� �����ϸ� utility�� foreach�� �ְ� ����Ʈ���� ���鼭 Ư�� component�� ����ȯ �ؼ� �Ѱ��ִ� �� ������� �ְڴ�.
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
//?!? �̰� PatchTextureLite�� �ű���. �׷��� �� ������ ��������.
void PatchTextureImpl::SetIndexedSource( size_t index, const vector<Rectangle>& sources,
	function<vector<Rectangle>(size_t idx)> SourceSelectorFn) noexcept
{
	ForEach<PatchTextureLite>([index, &sources, &SourceSelectorFn](PatchTextureLite* tex, size_t idx) {
		tex->SetIndexedSource(index, SourceSelectorFn(idx));
		return true;
		});
}
