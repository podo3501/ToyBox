#include "pch.h"
#include "PatchTextureLite9.h"
#include "PatchTextureLite3.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

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
		ReturnIfFalse(tex->SetupLayout(index, GetTripleAt(sources, idx)));
		UIEx(this).AttachComponent(move(tex), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	return ChangeSize(size, true);
}

void PatchTextureLite9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) { return GetTripleAt(sources, idx); });
}

vector<Rectangle> PatchTextureLite9::GetChildSourceList() const noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	return GetSourceList<PatchTextureLite3>(components, &PatchTextureLite3::GetFirstComponentSource);
}