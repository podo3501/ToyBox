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
    for (std::size_t idx = 0; idx < source.list.size(); ++idx) 
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

    for (int idx{ 0 }; idx < components.size(); ++idx)
    {
        ChangePosition(idx, posSizes[idx].pos);
        components[idx]->ChangeSize(posSizes[idx].size);
    }
    UIComponent::ChangeSize(size);
}

Rectangle ImageGrid3::GetFirstComponentSource() const noexcept
{
    const vector<UIComponent*> components = GetComponents();
    if (components.empty()) return {};
    
    const ImageGrid1* img1 = ComponentCast<ImageGrid1*>(components[0]);
    return img1->Source;
}