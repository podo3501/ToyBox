#include "pch.h"
#include "PatchTextureLite9.h"
#include "PatchTextureLite3.h"
#include "Utility.h"
#include "../../UIUtility.h"

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

bool PatchTextureLite9::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	ReturnIfFalse(sources.size() == 9);

	//vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_impl.GetDirectionType()); //?!?나중에 이 함수를 사용하는 걸로 바꾸자
	vector<optional<StateFlag::Type>> stateFlags{ StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };
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

bool PatchTextureLite9::ForEachPatchTextureLite3(predicate<PatchTextureLite3*, size_t> auto&& each)
{
	for (size_t index : views::iota(0u, 3u))
	{
		PatchTextureLite3* patchTexL3 = ComponentCast<PatchTextureLite3*>(GetChildComponent(index));
		if (!each(patchTexL3, index)) return false;
	}
	return true;
}

bool PatchTextureLite9::FitToTextureSource() noexcept
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachPatchTextureLite3([&totalSize, &sizes, this](PatchTextureLite3* tex3, size_t) {
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

void PatchTextureLite9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	ForEachPatchTextureLite3([index, &sources](PatchTextureLite3* tex3, size_t idx) {
		tex3->SetIndexedSource(index, { sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] });
		return true;
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

bool PatchTextureLite9::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachPatchTextureLite3([this, &sizes](PatchTextureLite3* tex3, size_t index) {
		return tex3->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTextureLite9::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, sizes);
	ReturnIfFalse(ForEachPatchTextureLite3([this, &size, &positions](PatchTextureLite3*, size_t index) {
		return ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTextureLite9::ImplementChangeSize(const XMUINT2& size) noexcept
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