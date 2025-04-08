#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "Utility.h"
#include "../UIUtility.h"

ImageGrid9::~ImageGrid9() = default;
ImageGrid9::ImageGrid9() = default;
ImageGrid9::ImageGrid9(const ImageGrid9& o) :
	ImageGrid{ o }
{}

unique_ptr<UIComponent> ImageGrid9::CreateClone() const
{
	return unique_ptr<ImageGrid9>(new ImageGrid9(*this));
}

bool ImageGrid9::Setup(const UILayout& layout, const string& bindKey)
{
	SetLayout(layout);

	vector<optional<StateFlag::Type>> stateFlags{ StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };
	for (size_t idx : views::iota(0, 3))
	{
		auto grid3 = CreateImageGrid3({ {}, Origin::LeftTop }, DirectionType::Horizontal, bindKey, idx);
		if (auto flag = stateFlags[idx]; flag) grid3->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(grid3), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
}

bool ImageGrid9::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	ChangeSize(GetSize(), true);
	UpdatePositionsManually();

	return true;
}

bool ImageGrid9::ForEachImageGrid3(predicate<ImageGrid3*, size_t> auto&& each)
{
	for (size_t index : views::iota(0u, 3u))
	{
		ImageGrid3* imgGrid3 = ComponentCast<ImageGrid3*>(GetChildComponent(index));
		if (!each(imgGrid3, index)) return false;
	}
	return true;
}

bool ImageGrid9::FitToTextureSource() noexcept
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachImageGrid3([&totalSize, &sizes, this](ImageGrid3* img3, size_t) {
		ReturnIfFalse(img3->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(img3->GetArea());
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

const string& ImageGrid9::GetBindKey() const noexcept
{
	ImageGrid3* imgGrid3 = ComponentCast<ImageGrid3*>(GetChildComponent(0));
	return imgGrid3->GetBindKey();
}

void ImageGrid9::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
	ForEachImageGrid3([&key, &sourceInfo](ImageGrid3* img3, size_t index) {
		img3->ChangeBindKey(key, sourceInfo, index);
		return true;
		});
}

void ImageGrid9::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	ForEachImageGrid3([index, &sources](ImageGrid3* img3, size_t idx) {
		img3->SetIndexedSource(index, { sources[idx * 3 + 0], sources[idx * 3 + 1], sources[idx * 3 + 2] });
		return true;
		});
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [](auto component) {
		ImageGrid3* image = ComponentCast<ImageGrid3*>(component);
		return image->GetFirstComponentSource();
		});
	return srcList;
}

bool ImageGrid9::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachImageGrid3([this, &sizes](ImageGrid3* img3, size_t index) {
		return img3->ChangeSize(sizes[index]);
		}));
	return true;
}

bool ImageGrid9::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, sizes);
	ReturnIfFalse(ForEachImageGrid3([this, &size, &positions](ImageGrid3*, size_t index) {
		return ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool ImageGrid9::ImplementChangeSize(const XMUINT2& size) noexcept
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

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const string& bindKey)
{
	auto img9 = make_unique<ImageGrid9>();
	return CreateIfSetup(move(img9), layout, bindKey);
}