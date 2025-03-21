#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "../../Utility.h"
#include "../UIUtility.h"
#include "ImageGridHelper.hpp"

ImageGrid9::~ImageGrid9() = default;
ImageGrid9::ImageGrid9() = default;

ImageGrid9::ImageGrid9(const ImageGrid9& o) :
	UIComponent{ o }
{
}

unique_ptr<UIComponent> ImageGrid9::CreateClone() const
{
	return unique_ptr<ImageGrid9>(new ImageGrid9(*this));
}

static inline vector<Rectangle> ExtractSourceRects(const ImageSource& source)
{
	return { source.list[0], source.list[3], source.list[6] };
}

static unique_ptr<ImageGrid3> CreateImageGrid3(DirectionType dirType, size_t idx, const ImageSource& source, const XMUINT2& size)
{
	UILayout grid3layout(size, Origin::LeftTop);
	ImageSource imgSource
	{
		source.filename,
		{ source.list[idx * 3], source.list[idx * 3 + 1], source.list[idx * 3 + 2] }
	};

	return CreateImageGrid3(dirType, grid3layout, imgSource);
}

bool ImageGrid9::Setup(const UILayout& layout, const ImageSource& source) noexcept
{
	if (source.filename.empty()) return false;
	if (source.list.size() != 9) return false;

	vector<optional<StateFlag::Type>> stateFlags{ StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };

	vector<XMUINT2> sizes; //source의 크기로 dest를 만든다.
	ranges::transform(source.list, back_inserter(sizes), [](auto& src) { return RectangleToXMUINT2(src); });
	vector<XMUINT2> firstSizes = { sizes[0], sizes[3], sizes[6] };
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, firstSizes);
	for (auto idx : views::iota(0u, positions.size()))
	{
		auto grid3 = CreateImageGrid3(DirectionType::Horizontal, idx, source, firstSizes[idx]);
		if (auto flag = stateFlags[idx]; flag) grid3->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(grid3), positions[idx]);
	}

	ChangeSize(layout.GetSize());
	SetLayout(layout);

	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	UpdatePositionsManually();

	return true;
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

bool ImageGrid9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	vector<XMUINT2> sizes = StretchSize(DirectionType::Vertical, size, components);
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, sizes);
	for (auto idx : views::iota(0u, components.size()))
	{
		ChangePosition(idx, size, positions[idx]);
		ReturnIfFalse(components[idx]->ChangeSize(sizes[idx]));
	}
	ApplySize(size);

	return true;
}

wstring ImageGrid9::GetFilename() const noexcept
{
	ImageGrid3* img3 = ComponentCast<ImageGrid3*>(GetChildComponent(0));
	if (!img3) return L"";

	return img3->GetFilename();
}

bool ImageGrid9::SetFilename(const wstring& filename) noexcept
{
	vector<ImageGrid3*> components;
	ReturnIfFalse(GetImageGridComponents(GetChildComponents(), components));

	return ranges::all_of(components, [&filename](auto imgGrid3) {
		return imgGrid3->SetFilename(filename);
		});
}

SourceDivider ImageGrid9::GetSourceAnd4Divider() const noexcept
{
	vector<ImageGrid3*> components;
	if (!GetImageGridComponents(GetChildComponents(), components)) return {};

	const Rectangle& firstMergedSource = components[0]->GetMergedSource();

	Rectangle mergedSource = firstMergedSource;
	for (auto imgGrid3 : components)
		mergedSource = Rectangle::Union(mergedSource, imgGrid3->GetMergedSource());//?!? utility에 CombineRectangles 있음

	SourceDivider srcDivider{};
	srcDivider.rect = mergedSource;

	//x값으로 2개 Divider를 담는다.
	auto firstComponent = components[0]->GetSourceAnd2Divider();
	if (firstComponent.Empty()) return {};
	srcDivider.list = firstComponent.list;

	//y값으로 2개 Divider를 담는다.
	const Rectangle& topSource = firstMergedSource;
	srcDivider.list.insert(srcDivider.list.end(), {
		topSource.y + topSource.height - mergedSource.y,
		components[2]->GetMergedSource().y - mergedSource.y
		});

	return srcDivider;
}

bool ImageGrid9::SetSources(const vector<Rectangle>& sources) noexcept
{
	vector<ImageGrid3*> components;
	ReturnIfFalse(GetImageGridComponents(GetChildComponents(), components));

	return ranges::all_of(components, [&sources, index = 0](auto component) mutable {
		vector<Rectangle> rowRects{ sources.begin() + index, sources.begin() + index + 3 }; index += 3;
		return component->SetSources(rowRects);
		});

	return true;
}

bool ImageGrid9::SetSourceAnd4Divider(const SourceDivider& srcDivider) noexcept
{
	vector<int> widths{}, heights{};
	ReturnIfFalse(GetSizeDividedByNine(srcDivider, widths, heights));

	vector<Rectangle> sources = GetSourcesFromArea(srcDivider.rect, widths, heights);
	if (sources.size() != 9) return false;

	return SetSources(sources);
}

vector<Rectangle> ImageGrid9::GetSources() const noexcept
{
	vector<ImageGrid3*> components;
	if (!GetImageGridComponents(GetChildComponents(), components)) return {};

	vector<Rectangle> areas;
	for (auto imgGrid3 : components)
		ranges::copy(imgGrid3->GetSources(), back_inserter(areas));

	return areas;
}

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const ImageSource& source)
{
	auto imgGrid9 = make_unique<ImageGrid9>();
	return CreateIfSetup(move(imgGrid9), layout, source);
}