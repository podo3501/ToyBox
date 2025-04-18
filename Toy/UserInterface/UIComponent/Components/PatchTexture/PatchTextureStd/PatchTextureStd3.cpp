#include "pch.h"
#include "PatchTextureStd3.h"
#include "PatchTextureStd1.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/JsonOperation/JsonOperation.h"

PatchTextureStd3::~PatchTextureStd3() = default;
PatchTextureStd3::PatchTextureStd3() noexcept :
    m_dirType{ DirectionType::Horizontal }
{
    m_impl.SetOwner(this);
}

PatchTextureStd3::PatchTextureStd3(const PatchTextureStd3& o) :
    PatchTextureStd{ o },
    m_dirType{ o.m_dirType }
{
    m_impl.SetOwner(this);
}

unique_ptr<UIComponent> PatchTextureStd3::CreateClone() const
{
    return unique_ptr<PatchTextureStd3>(new PatchTextureStd3(*this));
}

bool PatchTextureStd3::operator==(const UIComponent& rhs) const noexcept
{
    ReturnIfFalse(UIComponent::operator==(rhs));
    const PatchTextureStd3* o = static_cast<const PatchTextureStd3*>(&rhs);

    return (tie(m_dirType) == tie(o->m_dirType));
}

void PatchTextureStd3::SetDirectionType(DirectionType dirType) noexcept
{
    m_dirType = dirType;
    SetTextureSlice(DirTypeToTextureSlice(dirType));
}

bool PatchTextureStd3::Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept
{
    SetLayout({ layout.GetSize(), Origin::LeftTop });
    SetDirectionType(dirType);

    vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
    for (size_t idx : views::iota(0, 3)) //bindKey를 조회할 수 없어 빈 내용의 자식들을 생성한다.
    {
        size_t childSrcIdx = sourceIndex * 3 + idx;
        auto tex1 = CreatePatchTextureStd1({ {}, Origin::LeftTop }, bindKey, childSrcIdx);
        if (auto flag = stateFlags[idx]; flag) tex1->SetStateFlag(*flag, true);
        UIEx(this).AttachComponent(move(tex1), {});
    }
    SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

    return true;
}

bool PatchTextureStd3::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
    if (GetSize() == XMUINT2{}) //PatchTexture9을 만들면 초기 크기값이 0로 설정 돼 있다.
        SetSize(UIEx(this).GetChildrenBoundsSize());
    else
        ReturnIfFalse(ChangeSize(GetSize(), true));

    return true;
}

const string& PatchTextureStd3::GetBindKey() const noexcept
{
    PatchTextureStd1* patchTex1 = ComponentCast<PatchTextureStd1*>(GetChildComponent(0));
    return patchTex1->GetBindKey();
}

void PatchTextureStd3::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
    ChangeBindKeyWithIndex(key, sourceInfo, 0);
}

void PatchTextureStd3::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
    for (size_t index : views::iota(0u, 3u))
    {
        PatchTextureStd1* tex1 = ComponentCast<PatchTextureStd1*>(GetChildComponent(index));
        tex1->ChangeBindKeyWithIndex(key, sourceInfo, sourceIndex * 3 + index);
    }
}

void PatchTextureStd3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
    m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) {
        return vector<Rectangle>{ sources[idx] };
        });
}

bool PatchTextureStd3::ImplementChangeSize(const XMUINT2& size) noexcept
{
    const vector<UIComponent*> components = GetChildComponents();
    auto list = GetSourceList<PatchTextureStd1>(components, &PatchTextureStd1::GetSource);
    ReturnIfFalse(IsBiggerThanSource(m_dirType, size, list));

    return m_impl.ChangeSize(m_dirType, size, components);
}

bool PatchTextureStd3::FitToTextureSource() noexcept
{
    return m_impl.FitToTextureSource(m_dirType, [this](const XMUINT2& size, XMUINT2& totalSize) {
        switch (m_dirType) {
        case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
        case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
        }});
}

Rectangle PatchTextureStd3::GetFirstComponentSource() const noexcept
{
    PatchTextureStd1* tex1 = ComponentCast<PatchTextureStd1*>(GetChildComponent(0));
    if(!tex1) return {};

    return tex1->GetSource();
}

void PatchTextureStd3::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation); 
    operation.Process("DirectionType", m_dirType);

    if (operation.IsWrite()) return;
    SetTextureSlice(DirTypeToTextureSlice(m_dirType));
}

unique_ptr<PatchTextureStd3> CreatePatchTextureStd3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex)
{
    auto patchTex3 = make_unique<PatchTextureStd3>();
    return CreateIfSetup(move(patchTex3), layout, dirType, bindKey, sourceIndex);
}


