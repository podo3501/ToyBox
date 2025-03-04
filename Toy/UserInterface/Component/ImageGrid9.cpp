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

bool ImageGrid9::SetImage(const UILayout& layout, const ImageSource& source) noexcept
{
	if (source.filename.empty()) return false;
	if (source.list.size() != 9) return false;

	SetLayout(layout);

	auto srcHList = ExtractSourceRects(source);
	auto posRects = StretchSize(DirectionType::Vertical, layout.GetSize(), srcHList);
	for (size_t idx = 0; idx < srcHList.size(); ++idx)
	{
		auto grid3 = CreateImageGrid3(DirectionType::Horizontal, idx, source, posRects[idx].size);
		UIEx(this).AttachComponent(move(grid3), posRects[idx].pos);
	}
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

bool ImageGrid9::ChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	vector<PositionSize> posRects = StretchSize(DirectionType::Vertical, size, list);

	for (int idx{ 0 }; idx < components.size(); ++idx)
	{
		ReturnIfFalse(components[idx]->ChangeSize(posRects[idx].size));
		ChangePosition(idx, size, posRects[idx].pos);
	}
	ApplySize(size);

	return true;
}

optional<wstring> ImageGrid9::GetFilename() const noexcept
{
	ImageGrid3* img3 = ComponentCast<ImageGrid3*>(GetChildComponent(0));
	if (!img3) return nullopt;

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
		mergedSource = Rectangle::Union(mergedSource, imgGrid3->GetMergedSource());

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
	return imgGrid9->SetImage(layout, source) ? move(imgGrid9) : nullptr;
}