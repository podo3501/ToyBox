#include "pch.h"
#include "PatchTextureStd9.h"
#include "PatchTextureStd3.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

PatchTextureStd9::~PatchTextureStd9() = default;
PatchTextureStd9::PatchTextureStd9() noexcept :
	PatchTextureStd{ TextureSlice::Nine }
{}

PatchTextureStd9::PatchTextureStd9(const PatchTextureStd9& o) noexcept :
	PatchTextureStd{ o }
{}

unique_ptr<UIComponent> PatchTextureStd9::CreateClone() const
{
	return unique_ptr<PatchTextureStd9>(new PatchTextureStd9(*this));
}

bool PatchTextureStd9::Setup(const UILayout& layout, const string& bindKey)
{
	SetLayout(layout);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(DirectionType::Vertical);
	for (size_t idx : views::iota(0u, 3u))
	{
		auto tex1 = CreatePatchTextureStd3(DirectionType::Horizontal, bindKey, idx);
		if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(tex1), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
}

unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const UILayout& layout, const string& bindKey)
{
	auto patchTex9 = make_unique<PatchTextureStd9>();
	return CreateIfSetup(move(patchTex9), layout, bindKey);
}