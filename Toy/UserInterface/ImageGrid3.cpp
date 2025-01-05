#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "UIType.h"
#include "JsonOperation.h"

ImageGrid3::~ImageGrid3() = default;
ImageGrid3::ImageGrid3() = default;

ImageGrid3::ImageGrid3(const ImageGrid3& o) :
    UIComponent{ o }
{}

unique_ptr<UIComponent> ImageGrid3::CreateClone() const
{
    return unique_ptr<ImageGrid3>(new ImageGrid3(*this));
}

static bool ValidateInput(const string& name, const ImageSource& source)
{
    if (name.empty()) return false;
    if (source.filename.empty()) return false;
    if (source.list.size() != 3) return false;

    return true;
}

static unique_ptr<ImageGrid1> CreateImageGrid1(const string& baseName, size_t idx, 
    const ImageSource& source, const XMUINT2& size)
{
    const auto& grid1name = baseName + "_" + to_string(idx);
    UILayout grid1layout(size, Origin::LeftTop);

    ImageSource imgSource{ source.filename, { source.list[idx] } };

    auto grid1 = make_unique<ImageGrid1>();
    grid1->SetImage(grid1name, grid1layout, imgSource);

    return grid1;
}

bool ImageGrid3::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
    ReturnIfFalse(ValidateInput(name, source));

    Name = name;
	SetLayout(layout);

    vector<PositionSize> posSizes = StretchSize(StretchType::Width, layout.GetSize(), source.list);
    for(auto idx : views::iota(0u, source.list.size()))
    {
        auto grid1 = CreateImageGrid1(name, idx, source, posSizes[idx].size);
        AddComponent(move(grid1), posSizes[idx].pos);
    }

	return true;
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
    vector<Rectangle> srcList;
    ranges::transform(components, back_inserter(srcList), [](auto component) {
        ImageGrid1* image = ComponentCast<ImageGrid1*>(component);
        return image->Source;
        });
    return srcList;
}

void ImageGrid3::ChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetComponents();
    vector<Rectangle> list = GetSourceList(components);
    vector<PositionSize> posSizes = StretchSize(StretchType::Width, size, list);

    for (auto idx : views::iota(0u, components.size()))
    {
        ChangePosition(idx, posSizes[idx].pos);
        components[idx]->ChangeSize(posSizes[idx].size);
    }
    UIComponent::ChangeSize(size);
}

ImageGrid1* ImageGrid3::GetFirstImageGrid1() const noexcept
{
    vector<UIComponent*> components = GetComponents();
    if (components.empty()) return nullptr;

    return ComponentCast<ImageGrid1*>(components[0]);
}

Rectangle ImageGrid3::GetFirstComponentSource() const noexcept
{
    ImageGrid1* img1 = GetFirstImageGrid1();
    if(!img1) return {};

    return img1->Source;
}

bool ImageGrid3::GetFilename(wstring& outFilename) const noexcept
{
    ImageGrid1* img1 = GetFirstImageGrid1();
    if (!img1) return false;

    outFilename = img1->Filename.Get();
    return true;
}

bool ImageGrid3::SetFilename(const wstring& filename) noexcept
{
    vector<ImageGrid1*> components;
    ReturnIfFalse(GetImageGrid1Components(components));

    for (ImageGrid1* imgGrid1 : components)
        imgGrid1->Filename = filename;

    return true;
}

bool ImageGrid3::GetSourceAnd2Divider(SourceDivider& outSrcDivider) const noexcept
{
    vector<ImageGrid1*> components;
    ReturnIfFalse(GetImageGrid1Components(components));

    Rectangle sources = components[0]->Source;
    for (ImageGrid1* imgGrid1 : components)
        sources = Rectangle::Union(sources, imgGrid1->Source);

    outSrcDivider.rect = sources;
    const Rectangle& leftSource = components[0]->Source.Get();
    outSrcDivider.list.push_back(leftSource.x + leftSource.width - sources.x);   //x값 2개와 사각형 하나면 source 사각형 3개를 만들 수 있다.
    outSrcDivider.list.push_back(components[2]->Source.Get().x - sources.x);

    return true;
}

bool ImageGrid3::SetSources(const vector<Rectangle>& sources) noexcept
{
    vector<ImageGrid1*> components;
    ReturnIfFalse(GetImageGrid1Components(components));

    ranges::for_each(components, [&sources, index = 0](ImageGrid1* component) mutable {
        component->Source = sources[index++];
        });

    return true;
}

vector<Rectangle> ImageGrid3::GetSources() const noexcept
{
    vector<Rectangle> areas;

    vector<ImageGrid1*> components;
    if (!GetImageGrid1Components(components)) return {};

    for (ImageGrid1* imgGrid1 : components)
        areas.push_back(imgGrid1->Source);

    return areas;
}

bool ImageGrid3::SetSourceAnd2Divider(const SourceDivider& srcDivider) noexcept
{
    vector<int> threePoints(srcDivider.list);
    threePoints.push_back(srcDivider.rect.width);
    ranges::sort(threePoints);

    vector<int> widths{ 
        threePoints[0], 
        threePoints[1] - threePoints[0], 
        threePoints[2] - threePoints[1] 
    };

    vector<Rectangle> sources = GetSourcesFromAreaAndGaps(srcDivider.rect, widths);
    ReturnIfFalse(SetSources(sources));

    return true;
}

bool ImageGrid3::GetImageGrid1Components(vector<ImageGrid1*>& outComponents) const noexcept
{
    vector<UIComponent*> components = GetComponents();
    if (components.size() != 3) return false;

    outComponents.clear();
    for (UIComponent* component : components) 
    {
        ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(component);
        if (!imgGrid1) return false;
        outComponents.push_back(imgGrid1);
    }

    return true;
}