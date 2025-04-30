#include "pch.h"
#include "PatchTextureLite9.h"
#include "PatchTextureLite3.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

PatchTextureLite9::~PatchTextureLite9() = default;
PatchTextureLite9::PatchTextureLite9() noexcept :
	PatchTextureLite{ TextureSlice::Nine }
{}

PatchTextureLite9::PatchTextureLite9(const PatchTextureLite9& o) noexcept :
	PatchTextureLite{ o }
{}

unique_ptr<UIComponent> PatchTextureLite9::CreateClone() const
{
	return unique_ptr<PatchTextureLite9>(new PatchTextureLite9(*this));
}

bool PatchTextureLite9::Setup(const XMUINT2& size)
{
	SetLayout(size);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(DirectionType::Vertical);
	for (auto idx : views::iota(0u, 3u))
	{
		auto tex = CreatePatchTextureLite3({}, DirectionType::Horizontal);
		if (auto flag = stateFlags[idx]; flag) tex->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(tex), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
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

	if (size == XMUINT2{})
		return ArrangeTextures();

	return ChangeSize(size, true);
}

bool PatchTextureLite9::BindSourceInfo(size_t index, const vector<Rectangle>& sources)
{
	ReturnIfFalse(sources.size() == 9);

	for (auto idx : views::iota(0u, 3u))
	{
		auto texL = static_cast<PatchTextureLite*>(GetChildComponent(idx));
		texL->BindSourceInfo(index, GetTripleAt(sources, idx));
	}

	if (GetSize() == XMUINT2{})
		return ArrangeTextures();

	return ChangeSize(GetSize(), true);
}

unique_ptr<PatchTextureLite9> CreatePatchTextureLite9(const XMUINT2& size)
{
	return CreateIfSetup(make_unique<PatchTextureLite9>(), size);
}