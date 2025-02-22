#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../../Utility.h"
#include "../UIUtility.h"
#include "../JsonOperation.h"
#include "ImageGridHelper.hpp"

ImageGrid3::~ImageGrid3() = default;
ImageGrid3::ImageGrid3() = default;

ImageGrid3::ImageGrid3(const ImageGrid3& o) :
    UIComponent{ o }
{}

unique_ptr<UIComponent> ImageGrid3::CreateClone() const
{
    return unique_ptr<ImageGrid3>(new ImageGrid3(*this));
}

static unique_ptr<ImageGrid1> CreateImageGrid1(size_t idx, const ImageSource& source, const XMUINT2& size)
{
    UILayout grid1layout(size, Origin::LeftTop);
    ImageSource imgSource{ source.filename, { source.list[idx] } };

    auto grid1 = make_unique<ImageGrid1>();
    grid1->SetImage(grid1layout, imgSource);

    return grid1;
}

bool ImageGrid3::SetImage(const UILayout& layout, const ImageSource& source) noexcept
{
    if (source.filename.empty()) return false;
    if (source.list.size() != 3) return false;

	SetLayout(layout);

    vector<PositionSize> posSizes = StretchSize(StretchType::Width, layout.GetSize(), source.list);
    for(auto idx : views::iota(0u, source.list.size()))
    {
        auto grid1 = CreateImageGrid1(idx, source, posSizes[idx].size);
        UIEx(this).AttachComponent(move(grid1), posSizes[idx].pos);
    }
    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
    UpdatePositionsManually();

	return true;
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
    vector<Rectangle> srcList;
    ranges::transform(components, back_inserter(srcList), [](auto component) {
        ImageGrid1* image = ComponentCast<ImageGrid1*>(component);
        return image->GetSource();
        });
    return srcList;
}

void ImageGrid3::ChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetChildComponents();
    vector<Rectangle> list = GetSourceList(components);
    vector<PositionSize> posSizes = StretchSize(StretchType::Width, size, list);

    for (auto idx : views::iota(0u, components.size()))
    {
        ChangePosition(idx, size, posSizes[idx].pos);
        components[idx]->ChangeSize(posSizes[idx].size);
    }
    ApplySize(size);
}

Rectangle ImageGrid3::GetFirstComponentSource() const noexcept
{
    ImageGrid1* img1 = ComponentCast<ImageGrid1*>(GetChildComponent(0));
    if(!img1) return {};

    return img1->GetSource();
}

optional<wstring> ImageGrid3::GetFilename() const noexcept
{
    ImageGrid1* img1 = ComponentCast<ImageGrid1*>(GetChildComponent(0));
    if (!img1) return nullopt;

    return img1->GetFilename();
}

bool ImageGrid3::SetFilename(const wstring& filename) noexcept
{
    vector<ImageGrid1*> components;
    ReturnIfFalse(GetImageGridComponents(GetChildComponents(), components));

    for (auto imgGrid1 : components)
        imgGrid1->SetFilename(filename);

    return true;
}

optional<SourceDivider> ImageGrid3::GetSourceAnd2Divider() const noexcept
{
    vector<ImageGrid1*> components;
    if(!GetImageGridComponents(GetChildComponents(), components)) return nullopt;

    Rectangle mergedSource = GetMergedSource();

    SourceDivider srcDivider{};
    srcDivider.rect = mergedSource;
    const Rectangle& leftSource = components[0]->GetSource();
    srcDivider.list.push_back(leftSource.x + leftSource.width - mergedSource.x);   //x값 2개와 사각형 하나면 source 사각형 3개를 만들 수 있다.
    srcDivider.list.push_back(components[2]->GetSource().x - mergedSource.x);

    return srcDivider;
}

bool ImageGrid3::SetSources(const vector<Rectangle>& sources) noexcept
{
    vector<ImageGrid1*> components;
    ReturnIfFalse(GetImageGridComponents(GetChildComponents(), components));

    ranges::for_each(components, [&sources, index = 0](auto component) mutable {
        component->SetSource(sources[index++]);
        });

    return true;
}

Rectangle ImageGrid3::GetMergedSource() const noexcept
{
    return CombineRectangles(GetSources());
}

vector<Rectangle> ImageGrid3::GetSources() const noexcept
{
    vector<Rectangle> areas;

    vector<ImageGrid1*> components;
    if (!GetImageGridComponents(GetChildComponents(), components)) return {};

    for (auto imgGrid1 : components)
        areas.push_back(imgGrid1->GetSource());

    return areas;
}

bool ImageGrid3::SetSourceAnd2Divider(const SourceDivider& srcDivider) noexcept
{
    vector<int> widths{}, heights{};
    ReturnIfFalse(GetWidthsAndHeights(srcDivider, widths, heights));

    vector<Rectangle> sources = GetSourcesFromArea(srcDivider.rect, widths, heights);
    if (sources.size() != 3) return false;

    return SetSources(sources);
}