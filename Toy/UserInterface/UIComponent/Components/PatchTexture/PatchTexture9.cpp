#include "pch.h"
#include "PatchTexture9.h"
#include "PatchTexture3.h"
#include "Utility.h"
#include "../../UIUtility.h"

PatchTexture9::~PatchTexture9() = default;
PatchTexture9::PatchTexture9() noexcept :
	PatchTexture{ TextureSlice::Nine }
{}

PatchTexture9::PatchTexture9(const PatchTexture9& o) noexcept :
	PatchTexture{ o }
{}

unique_ptr<UIComponent> PatchTexture9::CreateClone() const
{
	return unique_ptr<PatchTexture9>(new PatchTexture9(*this));
}

bool PatchTexture9::Setup(const UILayout& layout, const string& bindKey)
{
	SetLayout(layout);

	vector<optional<StateFlag::Type>> stateFlags{ StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };
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

bool PatchTexture9::ForEachPatchTexture3(predicate<PatchTexture3*, size_t> auto&& each)
{
	for (size_t index : views::iota(0u, 3u))
	{
		PatchTexture3* patchTex3 = ComponentCast<PatchTexture3*>(GetChildComponent(index));
		if (!each(patchTex3, index)) return false;
	}
	return true;
}

bool PatchTexture9::FitToTextureSource() noexcept
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachPatchTexture3([&totalSize, &sizes, this](PatchTexture3* tex3, size_t) {
		ReturnIfFalse(tex3->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(tex3->GetArea());
		sizes.emplace_back(size);

		totalSize.y += size.y; 
		totalSize.x = max(totalSize.x, size.x);
		return true;
		});
	ReturnIfFalse(result);
	ReturnIfFalse(ApplyPositions(totalSize, sizes));

	SetSize(totalSize);
	return true;
}

const string& PatchTexture9::GetBindKey() const noexcept
{
	PatchTexture3* patchTex3 = ComponentCast<PatchTexture3*>(GetChildComponent(0));
	return patchTex3->GetBindKey();
}

void PatchTexture9::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
	ForEachPatchTexture3([&key, &sourceInfo](PatchTexture3* tex3, size_t index) {
		tex3->ChangeBindKeyWithIndex(key, sourceInfo, index);
		return true;
		});
}

void PatchTexture9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	ForEachPatchTexture3([index, &sources](PatchTexture3* tex3, size_t idx) {
		tex3->SetIndexedSource(index, { sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] });
		return true;
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

bool PatchTexture9::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachPatchTexture3([this, &sizes](PatchTexture3* tex3, size_t index) {
		return tex3->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTexture9::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, sizes);
	ReturnIfFalse(ForEachPatchTexture3([this, &size, &positions](PatchTexture3*, size_t index) {
		return ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTexture9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	vector<XMUINT2> sizes = StretchSize(DirectionType::Vertical, size, components);
	ReturnIfFalse(ApplyStretchSize(sizes));
	ReturnIfFalse(ApplyPositions(size, sizes));
	ApplySize(size);

	return true;
}

unique_ptr<PatchTexture9> CreatePatchTexture9(const UILayout& layout, const string& bindKey)
{
	auto patchTex9 = make_unique<PatchTexture9>();
	return CreateIfSetup(move(patchTex9), layout, bindKey);
}