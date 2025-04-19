#include "pch.h"
#include "PatchTexture3.h"
#include "PatchTexture1.h"
#include "Utility.h"
#include "../../UIUtility.h"
#include "../../../JsonOperation/JsonOperation.h"

PatchTexture3::~PatchTexture3() = default;
PatchTexture3::PatchTexture3() noexcept :
    m_dirType{ DirectionType::Horizontal }
{
    m_impl.SetOwner(this);
}

PatchTexture3::PatchTexture3(const PatchTexture3& o) :
    PatchTexture{ o },
    m_dirType{ o.m_dirType }
{
    m_impl.SetOwner(this);
}

unique_ptr<UIComponent> PatchTexture3::CreateClone() const
{
    return unique_ptr<PatchTexture3>(new PatchTexture3(*this));
}

bool PatchTexture3::operator==(const UIComponent& rhs) const noexcept
{
    ReturnIfFalse(UIComponent::operator==(rhs));
    const PatchTexture3* o = static_cast<const PatchTexture3*>(&rhs);

    return (tie(m_dirType) == tie(o->m_dirType));
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

bool PatchTexture3::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
    if (GetSize() == XMUINT2{}) //PatchTexture9을 만들면 초기 크기값이 0로 설정 돼 있다.
        SetSize(UIEx(this).GetChildrenBoundsSize());
    else
        ChangeSize(GetSize(), true);
    UpdatePositionsManually(); //?!? 이게 필요할까 모르겠네

    return true;
}

const string& PatchTexture3::GetBindKey() const noexcept
{
    PatchTexture1* patchTex1 = ComponentCast<PatchTexture1*>(GetChildComponent(0));
    return patchTex1->GetBindKey();
}

void PatchTexture3::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
    ChangeBindKeyWithIndex(key, sourceInfo, 0);
}

void PatchTexture3::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
    for (size_t index : views::iota(0u, 3u))
    {
        PatchTexture1* tex1 = ComponentCast<PatchTexture1*>(GetChildComponent(index));
        tex1->ChangeBindKeyWithIndex(key, sourceInfo, sourceIndex * 3 + index);
    }
}

void PatchTexture3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
    m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) {
        return vector<Rectangle>{ sources[idx] };
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

bool PatchTexture3::ImplementChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetChildComponents();
    vector<Rectangle> list = GetSourceList(components);
    ReturnIfFalse(IsBiggerThanSource(m_dirType, size, list));

    return m_impl.ChangeSize(m_dirType, size, components);
}

bool PatchTexture3::FitToTextureSource() noexcept
{
    return m_impl.FitToTextureSource(m_dirType, [this](const XMUINT2& size, XMUINT2& totalSize) {
        switch (m_dirType) {
        case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
        case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
        }});
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

    if (operation.IsWrite()) return;
    SetTextureSlice(DirTypeToTextureSlice(m_dirType));
}

unique_ptr<PatchTexture3> CreatePatchTexture3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex)
{
    auto patchTex3 = make_unique<PatchTexture3>();
    return CreateIfSetup(move(patchTex3), layout, dirType, bindKey, sourceIndex);
}


