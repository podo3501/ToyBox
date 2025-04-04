#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../../../Utility.h"
#include "../UIUtility.h"
#include "../../JsonOperation/JsonOperation.h"
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

bool ImageGrid3::Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept
{
    SetLayout({ layout.GetSize(), Origin::LeftTop });
    m_dirType = dirType;

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

bool ImageGrid3::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
    if (GetSize() == XMUINT2{}) //ImageGrid9을 만들면 초기 크기값이 0로 설정 돼 있다.
        SetSize(UIEx(this).GetTotalChildSize());
    else
        ChangeSize(GetSize(), true);
    UpdatePositionsManually();

    return true;
}

const string& ImageGrid3::GetBindKey() const noexcept
{
    ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(GetChildComponent(0));
    return imgGrid1->GetBindKey();
}

void ImageGrid3::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
    for (size_t index : views::iota(0u, 3u))
    {
        ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(GetChildComponent(index));
        imgGrid1->ChangeBindKey(key, sourceInfo, sourceIndex * 3 + index);
    }
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

void ImageGrid3::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation); 
    operation.Process("DirectionType", m_dirType);
}

//?!? component를 넣는 식으로 바꾸자. 함수 인자를 계속해서 전달할 필요는 없으니까.
unique_ptr<ImageGrid3> CreateImageGrid3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex)
{
    auto grid3 = make_unique<ImageGrid3>();
    return CreateIfSetup(move(grid3), layout, dirType, bindKey, sourceIndex);
}