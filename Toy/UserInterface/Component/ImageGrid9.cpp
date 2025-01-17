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

static bool ValidateInput(const string& name, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 9) return false;

	return true;
}

static inline vector<Rectangle> ExtractSourceRects(const ImageSource& source)
{
	return { source.list[0], source.list[3], source.list[6] };
}

static unique_ptr<ImageGrid3> CreateImageGrid3(const string& name, size_t idx,
	const ImageSource& source, const XMUINT2& size)
{
	const auto& grid3name = name + "_" + to_string(idx);
	UILayout grid3layout(size, Origin::LeftTop);

	ImageSource imgSource
	{
		source.filename,
		{ source.list[idx * 3], source.list[idx * 3 + 1], source.list[idx * 3 + 2] }
	};

	auto grid3 = make_unique<ImageGrid3>();
	grid3->SetImage(grid3name, grid3layout, imgSource);

	return grid3;
}

bool ImageGrid9::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	ReturnIfFalse(ValidateInput(name, source));

	SetName(name);
	SetLayout(layout);

	auto srcHList = ExtractSourceRects(source);
	auto posRects = StretchSize(StretchType::Height, layout.GetSize(), srcHList);
	for (size_t idx = 0; idx < srcHList.size(); ++idx)
	{
		auto grid3 = CreateImageGrid3(name, idx, source, posRects[idx].size);
		grid3->SetAttachmentState(AttachmentState::Disable);
		AttachComponent(move(grid3), posRects[idx].pos);
	}
	SetAttachmentState(AttachmentState::Detach);

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

void ImageGrid9::ChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetComponents();
	vector<Rectangle> list = GetSourceList(components);
	vector<PositionSize> posRects = StretchSize(StretchType::Height, size, list);

	for (int idx{ 0 }; idx < components.size(); ++idx)
	{
		ChangePosition(idx, size, posRects[idx].pos);
		components[idx]->ChangeSize(posRects[idx].size);
	}
	ApplySize(size);
}

bool ImageGrid9::GetFilename(wstring& outFilename) const noexcept
{
	ImageGrid3* img3 = GetFirstImageGrid<ImageGrid3*>(this);
	if (!img3) return false;

	return img3->GetFilename(outFilename);
}

bool ImageGrid9::SetFilename(const wstring& filename) noexcept
{
	vector<ImageGrid3*> components;
	ReturnIfFalse(GetImageGridComponents<ImageGrid3*>(this, components));

	return ranges::all_of(components, [&filename](auto imgGrid3) {
		return imgGrid3->SetFilename(filename);
		});
}

bool ImageGrid9::GetSourceAnd4Divider(SourceDivider& outSrcDivider) const noexcept
{
	vector<ImageGrid3*> components;
	ReturnIfFalse(GetImageGridComponents<ImageGrid3*>(this, components));

	const Rectangle& firstMergedSource = components[0]->GetMergedSource();

	Rectangle mergedSource = firstMergedSource;
	for (auto imgGrid3 : components)
		mergedSource = Rectangle::Union(mergedSource, imgGrid3->GetMergedSource());

	outSrcDivider.rect = mergedSource;

	//x값으로 2개 Divider를 담는다.
	SourceDivider firstComponent;
	components[0]->GetSourceAnd2Divider(firstComponent);
	outSrcDivider.list = firstComponent.list;

	//y값으로 2개 Divider를 담는다.
	const Rectangle& topSource = firstMergedSource;
	outSrcDivider.list.insert(outSrcDivider.list.end(), {
		topSource.y + topSource.height - mergedSource.y,
		components[2]->GetMergedSource().y - mergedSource.y
		});

	return true;
}

bool ImageGrid9::SetSources(const vector<Rectangle>& sources) noexcept
{
	vector<ImageGrid3*> components;
	ReturnIfFalse(GetImageGridComponents<ImageGrid3*>(this, components));

	return ranges::all_of(components, [&sources, index = 0](auto component) mutable {
		vector<Rectangle> rowRects{ sources.begin() + index, sources.begin() + index + 3 }; index += 3;
		return component->SetSources(rowRects);
		});

	return true;
}

bool ImageGrid9::SetSourceAnd4Divider(const SourceDivider& srcDivider) noexcept
{
	vector<int> widths{}, heights{};
	ReturnIfFalse(GetWidthsAndHeights(srcDivider, widths, heights));

	vector<Rectangle> sources = GetSourcesFromArea(srcDivider.rect, widths, heights);
	if (sources.size() != 9) return false;

	return SetSources(sources);
}

vector<Rectangle> ImageGrid9::GetSources() const noexcept
{
	vector<ImageGrid3*> components;
	if (!GetImageGridComponents<ImageGrid3*>(this, components)) return {};

	vector<Rectangle> areas;
	for (auto imgGrid3 : components)
		ranges::copy(imgGrid3->GetSources(), back_inserter(areas));

	return areas;
}