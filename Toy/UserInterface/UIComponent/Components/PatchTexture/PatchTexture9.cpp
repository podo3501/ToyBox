#include "pch.h"
#include "PatchTexture9.h"
#include "PatchTexture3.h"
#include "Utility.h"
#include "../../UIUtility.h"

PatchTexture9::~PatchTexture9() = default;
PatchTexture9::PatchTexture9() noexcept :
	PatchTexture{ TextureSlice::Nine }
{
	m_impl.SetOwner(this);
}

PatchTexture9::PatchTexture9(const PatchTexture9& o) noexcept :
	PatchTexture{ o }
{
	m_impl.SetOwner(this);
}

unique_ptr<UIComponent> PatchTexture9::CreateClone() const
{
	return unique_ptr<PatchTexture9>(new PatchTexture9(*this));
}

bool PatchTexture9::Setup(const UILayout& layout, const string& bindKey)
{
	SetLayout(layout);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(DirectionType::Vertical);
	for (size_t idx : views::iota(0u, 3u))
	{
		auto tex1 = CreatePatchTexture3({ {}, Origin::LeftTop }, DirectionType::Horizontal, bindKey, idx);
		if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(tex1), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
}

bool PatchTexture9::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	ReturnIfFalse(ChangeSize(GetSize(), true));
	ReturnIfFalse(UpdatePositionsManually());

	return true;
}

bool PatchTexture9::FitToTextureSource() noexcept
{
	return m_impl.FitToTextureSource(DirectionType::Vertical, [this](const XMUINT2& size, XMUINT2& totalSize) {
		totalSize.y += size.y;
		totalSize.x = max(totalSize.x, size.x);
		});
}

const string& PatchTexture9::GetBindKey() const noexcept
{
	PatchTexture3* patchTex3 = ComponentCast<PatchTexture3*>(GetChildComponent(0));
	return patchTex3->GetBindKey();
}

void PatchTexture9::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
	for (size_t index : views::iota(0u, 3u))
	{
		PatchTexture3* tex3 = ComponentCast<PatchTexture3*>(GetChildComponent(index));
		tex3->ChangeBindKeyWithIndex(key, sourceInfo, index);
	}
}

void PatchTexture9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) {
		return vector<Rectangle>{ sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] };
		});
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [](auto component) {
		PatchTexture3* tex = ComponentCast<PatchTexture3*>(component);
		return tex->GetFirstComponentSource();
		});
	return srcList;
}

bool PatchTexture9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	return m_impl.ChangeSize(DirectionType::Vertical, size, components);
}

unique_ptr<PatchTexture9> CreatePatchTexture9(const UILayout& layout, const string& bindKey)
{
	auto patchTex9 = make_unique<PatchTexture9>();
	return CreateIfSetup(move(patchTex9), layout, bindKey);
}