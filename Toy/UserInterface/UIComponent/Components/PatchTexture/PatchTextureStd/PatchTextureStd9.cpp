#include "pch.h"
#include "PatchTextureStd9.h"
#include "PatchTextureStd3.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"

PatchTextureStd9::~PatchTextureStd9() = default;
PatchTextureStd9::PatchTextureStd9() noexcept :
	PatchTextureStd{ TextureSlice::Nine }
{
	m_impl.SetOwner(this);
}

PatchTextureStd9::PatchTextureStd9(const PatchTextureStd9& o) noexcept :
	PatchTextureStd{ o }
{
	m_impl.SetOwner(this);
}

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
		auto tex1 = CreatePatchTextureStd3({ {}, Origin::LeftTop }, DirectionType::Horizontal, bindKey, idx);
		if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(tex1), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
}

bool PatchTextureStd9::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	return ChangeSize(GetSize(), true);
}

bool PatchTextureStd9::FitToTextureSource() noexcept
{
	return m_impl.FitToTextureSource(DirectionType::Vertical, [this](const XMUINT2& size, XMUINT2& totalSize) {
		totalSize.y += size.y;
		totalSize.x = max(totalSize.x, size.x);
		});
}

const string& PatchTextureStd9::GetBindKey() const noexcept
{
	PatchTextureStd3* patchTex3 = ComponentCast<PatchTextureStd3*>(GetChildComponent(0));
	return patchTex3->GetBindKey();
}

bool PatchTextureStd9::ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept
{
	auto infoRef = resBinder->GetTextureSourceInfo(key);
	ReturnIfFalse(infoRef);

	for (size_t index : views::iota(0u, 3u))
	{
		PatchTextureStd3* tex3 = ComponentCast<PatchTextureStd3*>(GetChildComponent(index));
		tex3->ChangeBindKeyWithIndex(key, *infoRef, index);
	}

	return FitToTextureSource();
}

bool PatchTextureStd9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	auto list = GetSourceList<PatchTextureStd3>(components, &PatchTextureStd3::GetFirstComponentSource);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	return m_impl.ChangeSize(DirectionType::Vertical, size, components);
}

unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const UILayout& layout, const string& bindKey)
{
	auto patchTex9 = make_unique<PatchTextureStd9>();
	return CreateIfSetup(move(patchTex9), layout, bindKey);
}