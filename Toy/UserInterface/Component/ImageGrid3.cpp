#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../../Utility.h"
#include "../UIUtility.h"
#include "../JsonOperation.h"
#include "ImageGridHelper.hpp"

ImageGrid3::~ImageGrid3() = default;
ImageGrid3::ImageGrid3() = default;
ImageGrid3::ImageGrid3(DirectionType dirType) :
    m_dirType{ dirType }
{}

ImageGrid3::ImageGrid3(const ImageGrid3& o) :
    ImageGrid{ o },
    m_dirType{ o.m_dirType }
{}

unique_ptr<UIComponent> ImageGrid3::CreateClone() const
{
    return unique_ptr<ImageGrid3>(new ImageGrid3(*this));
}

bool ImageGrid3::operator==(const UIComponent& rhs) const noexcept
{
    ReturnIfFalse(UIComponent::operator==(rhs));
    const ImageGrid3* o = static_cast<const ImageGrid3*>(&rhs);

    return(tie(m_dirType) == tie(o->m_dirType));
}

static vector<optional<StateFlag::Type>> GetStateFlagsForDirection(DirectionType dirType) noexcept
{
    switch (dirType)
    {
    case DirectionType::Horizontal: return { StateFlag::X_SizeLocked, nullopt, StateFlag::X_SizeLocked };
    case DirectionType::Vertical: return { StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };
    }
    return {};
}

static pair<vector<XMUINT2>, vector<XMINT2>> ComputeBoundsFromSources(DirectionType dirType, const vector<Rectangle>& sources) noexcept
{
    vector<XMUINT2> srcSizes; //source의 크기로 dest를 만든다.
    ranges::transform(sources, back_inserter(srcSizes), [](auto& src) { return RectangleToXMUINT2(src); });
    vector<XMINT2> positions = ExtractStartPosFromSizes(dirType, srcSizes);

    return make_pair(srcSizes, positions);
}

void ImageGrid3::SetupDetails(const XMUINT2& size) noexcept
{
    ChangeSize(size);
    SetLayout({ size, Origin::LeftTop });

    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
    UpdatePositionsManually();
}

bool ImageGrid3::Setup(DirectionType dirType, const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept
{
    m_dirType = dirType;
    SetLayout({ layout.GetSize(), Origin::LeftTop });

    vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
    for (size_t idx : views::iota(0, 3)) //bindKey를 조회할 수 없어 빈 내용의 자식들을 생성한다.
    {
        size_t childSrcIdx = sourceIndex * 3 + idx;
        auto grid1 = CreateImageGrid1({ {}, Origin::LeftTop }, bindKey, childSrcIdx);
        if (auto flag = stateFlags[idx]; flag) grid1->SetStateFlag(*flag, true);
        UIEx(this).AttachComponent(move(grid1), {});
    }
    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

    return true;
}

bool ImageGrid3::ImplementBindSourceInfo(TextureSourceBinder*, ITextureController*) noexcept
{
    if (GetSize() == XMUINT2{}) //ImageGrid9을 만들면 초기 크기값이 0로 설정 돼 있다.
        SetSize(UIEx(this).GetTotalChildSize());
    else
        ChangeSize(GetSize(), true);
    UpdatePositionsManually();

    return true;
}

void ImageGrid3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
    for (auto idx : views::iota(0u, sources.size()))
    {
        if (auto img1 = ComponentCast<ImageGrid1*>(GetChildComponent(idx)); img1)
            img1->SetIndexedSource(index, { sources[idx] });
    }
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

bool ImageGrid3::ImplementChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetChildComponents();
    vector<Rectangle> list = GetSourceList(components);
    ReturnIfFalse(IsBiggerThanSource(m_dirType, size, list));

    vector<XMUINT2> sizes = StretchSize(m_dirType, size, components);
    vector<XMINT2> positions = ExtractStartPosFromSizes(m_dirType, sizes);
    for (auto idx : views::iota(0u, components.size()))
    {
        ChangePosition(idx, size, positions[idx]);
        ReturnIfFalse(components[idx]->ChangeSize(sizes[idx]));
    }
    ApplySize(size);
    
    return true;
}

Rectangle ImageGrid3::GetFirstComponentSource() const noexcept
{
    ImageGrid1* img1 = ComponentCast<ImageGrid1*>(GetChildComponent(0));
    if(!img1) return {};

    return img1->GetSource();
}

wstring ImageGrid3::GetFilename() const noexcept
{
    ImageGrid1* img1 = ComponentCast<ImageGrid1*>(GetChildComponent(0));
    if (!img1) return L"";

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

SourceDivider ImageGrid3::GetSourceAnd2Divider() const noexcept
{
    vector<ImageGrid1*> components;
    if (!GetImageGridComponents(GetChildComponents(), components)) return {};

    Rectangle mergedSource = GetMergedSource();

    SourceDivider srcDivider{};
    srcDivider.rect = mergedSource;
    const Rectangle& leftSource = components[0]->GetSource();
    switch (m_dirType) //값 2개와 사각형 하나면 source 사각형 3개를 만들 수 있다.
    {
    case DirectionType::Horizontal:
        srcDivider.list.push_back(leftSource.x + leftSource.width - mergedSource.x);   
        srcDivider.list.push_back(components[2]->GetSource().x - mergedSource.x);
        break;
    case DirectionType::Vertical:
        srcDivider.list.push_back(leftSource.y + leftSource.height - mergedSource.y);   
        srcDivider.list.push_back(components[2]->GetSource().y - mergedSource.y);
        break;
    }
    
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
    ReturnIfFalse(GetSizeDividedByThree(m_dirType, srcDivider, widths, heights));

    vector<Rectangle> sources = GetSourcesFromArea(srcDivider.rect, widths, heights);
    if (sources.size() != 3) return false;

    return SetSources(sources);
}

void ImageGrid3::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation); 
    operation.Process("DirectionType", m_dirType);
}

//?!? component를 넣는 식으로 바꾸자. 함수 인자를 계속해서 전달할 필요는 없으니까.
unique_ptr<ImageGrid3> CreateImageGrid3(DirectionType dirType, const UILayout& layout, const string& bindKey, size_t sourceIndex)
{
    auto grid3 = make_unique<ImageGrid3>();
    return CreateIfSetup(move(grid3), dirType, layout, bindKey, sourceIndex);
}