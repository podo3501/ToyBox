#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"
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
    const ImageSource& source, const PositionRectangle& posRect)
{
    const auto& grid1name = baseName + "_" + to_string(idx);
    UILayout grid1layout(posRect.area, Origin::LeftTop);

    ImageSource imgSource{ source.filename, { source.list[idx] } };

    auto grid1 = make_unique<ImageGrid1>();
    grid1->SetImage(grid1name, grid1layout, imgSource);

    return grid1;
}

bool ImageGrid3::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
    ReturnIfFalse(ValidateInput(name, source));

	SetName(name);
	SetLayout(layout);

    vector<PositionRectangle> posRects = StretchSize(StretchType::Width, layout.GetArea(), source.list);
    for (std::size_t idx = 0; idx < source.list.size(); ++idx) 
    {
        auto grid1 = CreateImageGrid1(name, idx, source, posRects[idx]);
        AddComponent(move(grid1), posRects[idx].pos);	//rectangle이 아니라 XMINT로 해야 하지 않나? x, y 값은 언제나 0이다.
    }

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

bool ImageGrid3::ChangeArea(const Rectangle& area) noexcept
{
    const vector<UIComponent*> components = GetComponents();
    vector<Rectangle> list = GetSourceList(components);
    vector<PositionRectangle> posRects = StretchSize(StretchType::Width, area, list);

    for (int idx{ 0 }; idx < components.size(); ++idx)
    {
        ChangePosition(idx, posRects[idx].pos);
        components[idx]->ChangeArea(posRects[idx].area);
    }
    UIComponent::ChangeArea(area);

    return true;
}

Rectangle ImageGrid3::GetFirstComponentSource() const noexcept
{
    const vector<UIComponent*> components = GetComponents();
    if (components.empty()) return {};
    
    const ImageGrid1* img1 = ComponentCast<ImageGrid1*>(components[0]);
    return img1->GetSource();
}

unique_ptr<ImageGrid3> CreateImageGrid3(const string& name, const UILayout& layout, const ImageSource& source)
{
    auto imgGrid3 = make_unique<ImageGrid3>();
    if (!imgGrid3->SetImage(name, layout, source)) return nullptr;

    return imgGrid3;
}