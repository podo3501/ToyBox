#include "pch.h"
#include "PatchTexture3.h"
#include "PatchTexture1.h"
#include "Utility.h"
#include "../../UIUtility.h"
#include "../../../JsonOperation/JsonOperation.h"

PatchTexture3::~PatchTexture3() = default;
PatchTexture3::PatchTexture3() noexcept = default;
PatchTexture3::PatchTexture3(const PatchTexture3& o) :
    PatchTexture{ o },
    m_dirType{ o.m_dirType }
{}

unique_ptr<UIComponent> PatchTexture3::CreateClone() const
{
    return unique_ptr<PatchTexture3>(new PatchTexture3(*this));
}

bool PatchTexture3::operator==(const UIComponent& rhs) const noexcept
{
    ReturnIfFalse(UIComponent::operator==(rhs));
    const PatchTexture3* o = static_cast<const PatchTexture3*>(&rhs);

    return(tie(m_dirType) == tie(o->m_dirType));
}

void PatchTexture3::SetDirectionType(DirectionType dirType) noexcept
{
    m_dirType = dirType;
    SetTextureSlice(DirTypeToTextureSlice(dirType));
}

bool PatchTexture3::Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept
{
    SetLayout({ layout.GetSize(), Origin::LeftTop });
    SetDirectionType(dirType);

    vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
    for (size_t idx : views::iota(0, 3)) //bindKey를 조회할 수 없어 빈 내용의 자식들을 생성한다.
    {
        size_t childSrcIdx = sourceIndex * 3 + idx;
        auto tex1 = CreatePatchTexture1({ {}, Origin::LeftTop }, bindKey, childSrcIdx);
        if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
        UIEx(this).AttachComponent(move(tex1), {});
    }
    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

    return true;
}

bool PatchTexture3::SetupWithoutBindKey(const UILayout& layout, DirectionType dirType) noexcept
{
    SetLayout({ layout.GetSize(), Origin::LeftTop });
    SetDirectionType(dirType);

    vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
    for (size_t idx : views::iota(0, 3))
    {
        auto tex1 = CreatePatchTexture1Lite({ {}, Origin::LeftTop });
        if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
        UIEx(this).AttachComponent(move(tex1), {});
    }
    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

    return true;
}

bool PatchTexture3::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
    if (GetSize() == XMUINT2{}) //PatchTexture9을 만들면 초기 크기값이 0로 설정 돼 있다.
        SetSize(UIEx(this).GetChildrenBoundsSize());
    else
        ChangeSize(GetSize(), true);
    UpdatePositionsManually();

    return true;
}

const string& PatchTexture3::GetBindKey() const noexcept
{
    PatchTexture1* patchTex1 = ComponentCast<PatchTexture1*>(GetChildComponent(0));
    return patchTex1->GetBindKey();
}

bool PatchTexture3::ForEachPatchTexture1(predicate<PatchTexture1*, size_t> auto&& each)
{
    for (size_t index : views::iota(0u, 3u))
    {
        PatchTexture1* patchTex1 = ComponentCast<PatchTexture1*>(GetChildComponent(index));
        if (!each(patchTex1, index)) return false;
    }
    return true;
}

void PatchTexture3::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
    ChangeBindKeyWithIndex(key, sourceInfo, 0);
}

void PatchTexture3::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
    ForEachPatchTexture1([&key, &sourceInfo, sourceIndex](PatchTexture1* tex1, size_t index) {
        tex1->ChangeBindKeyWithIndex(key, sourceInfo, sourceIndex * 3 + index);
        return true;
        });
}

void PatchTexture3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
    ForEachPatchTexture1([index, &sources](PatchTexture1* tex1, size_t idx) {
        tex1->SetIndexedSource(index, { sources[idx] });
        return true;
        });
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
    vector<Rectangle> srcList;
    ranges::transform(components, back_inserter(srcList), [](auto component) {
        PatchTexture1* tex = ComponentCast<PatchTexture1*>(component);
        return tex->GetSource();
        });
    return srcList;
}

bool PatchTexture3::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
    ReturnIfFalse(ForEachPatchTexture1([this, &sizes](PatchTexture1* tex1, size_t index) {
        return tex1->ChangeSize(sizes[index]);
        }));
    return true;
}

bool PatchTexture3::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
    vector<XMINT2> positions = ExtractStartPosFromSizes(m_dirType, sizes);
    ReturnIfFalse(ForEachPatchTexture1([this, &size, &positions](PatchTexture1*, size_t index) {
        return ChangePosition(index, size, positions[index]);
        }));
    return true;
}

bool PatchTexture3::ImplementChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetChildComponents();
    vector<Rectangle> list = GetSourceList(components);
    ReturnIfFalse(IsBiggerThanSource(m_dirType, size, list));

    //여기서  자식들의 기본 사이즈가 없어서 stretch가 되지 않았다.
    vector<XMUINT2> sizes = StretchSize(m_dirType, size, GetChildComponents());
    ReturnIfFalse(ApplyStretchSize(sizes));
    ReturnIfFalse(ApplyPositions(size, sizes));
    ApplySize(size);
    
    return true;
}

bool PatchTexture3::FitToTextureSource() noexcept
{
    XMUINT2 totalSize{};
    vector<XMUINT2> sizes{};
    auto result = ForEachPatchTexture1([&totalSize, &sizes, this](PatchTexture1* tex1, size_t) {
        ReturnIfFalse(tex1->FitToTextureSource());
        const XMUINT2 size = GetSizeFromRectangle(tex1->GetArea());
        sizes.emplace_back(size);
        switch (m_dirType) {
        case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
        case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
        }
        return true;
        });
    ReturnIfFalse(result); //result 변수를 써서 ReturnIfFalse를 한 이유는 람다에 디버깅(브레이크 포인터)이 안되기 때문이다.
    ReturnIfFalse(ApplyPositions(totalSize, sizes));

    SetSize(totalSize);
    return true;
}

Rectangle PatchTexture3::GetFirstComponentSource() const noexcept
{
    PatchTexture1* tex1 = ComponentCast<PatchTexture1*>(GetChildComponent(0));
    if(!tex1) return {};

    return tex1->GetSource();
}

void PatchTexture3::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation); 
    operation.Process("DirectionType", m_dirType);
}

unique_ptr<PatchTexture3> CreatePatchTexture3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex)
{
    auto patchTex3 = make_unique<PatchTexture3>();
    return CreateIfSetup(move(patchTex3), layout, dirType, bindKey, sourceIndex);
}

unique_ptr<PatchTexture3> CreateUnboundPatchTexture3(const UILayout& layout, DirectionType dirType)
{
    auto patchTex3 = make_unique<PatchTexture3>();
    if (!patchTex3->SetupWithoutBindKey(layout, dirType)) return nullptr;

    return move(patchTex3);
}


