#include "pch.h"
#include "PatchTextureLite9.h"
#include "PatchTextureLite3.h"
#include "Utility.h"
#include "../../UIUtility.h"

PatchTextureLite9::~PatchTextureLite9() = default;
PatchTextureLite9::PatchTextureLite9() noexcept :
	PatchTextureLite{ TextureSlice::Nine }
{
	m_impl.SetOwner(this);
}

PatchTextureLite9::PatchTextureLite9(const PatchTextureLite9& o) noexcept :
	PatchTextureLite{ o }
{
	m_impl.SetOwner(this);
}

unique_ptr<UIComponent> PatchTextureLite9::CreateClone() const
{
	return unique_ptr<PatchTextureLite9>(new PatchTextureLite9(*this));
}

bool PatchTextureLite9::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	ReturnIfFalse(sources.size() == 9);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(DirectionType::Vertical);
	for (auto idx : views::iota(0u, 3u))
	{
		unique_ptr<PatchTextureLite> tex = make_unique<PatchTextureLite3>();
		if (auto flag = stateFlags[idx]; flag) tex->SetStateFlag(*flag, true);
		ReturnIfFalse(tex->SetupLayout(index, { sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] }));
		UIEx(this).AttachComponent(move(tex), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	return ChangeSize(size, true);
}

bool PatchTextureLite9::FitToTextureSource() noexcept
{
	return m_impl.FitToTextureSource(DirectionType::Vertical, [this](const XMUINT2& size, XMUINT2& totalSize) {
		totalSize.y += size.y;
		totalSize.x = max(totalSize.x, size.x);
		});
}

void PatchTextureLite9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) {
		return vector<Rectangle>{ sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] };
		});
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [](auto component) {
		PatchTextureLite3* tex = ComponentCast<PatchTextureLite3*>(component);
		return tex->GetFirstComponentSource();
		});
	return srcList;
}

bool PatchTextureLite9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	return m_impl.ChangeSize(DirectionType::Vertical, size, components);
}