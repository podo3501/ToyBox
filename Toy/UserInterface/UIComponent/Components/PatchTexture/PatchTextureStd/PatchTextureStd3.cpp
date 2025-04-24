#include "pch.h"
#include "PatchTextureStd3.h"
#include "PatchTextureStd1.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/JsonOperation/JsonOperation.h"

PatchTextureStd3::~PatchTextureStd3() = default;
PatchTextureStd3::PatchTextureStd3() noexcept :
    PatchTextureStd{ TextureSlice::ThreeH },
    m_dirType{ DirectionType::Horizontal }
{}

PatchTextureStd3::PatchTextureStd3(const PatchTextureStd3& o) :
    PatchTextureStd{ o },
    m_dirType{ o.m_dirType }
{}

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

bool PatchTextureStd3::ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept
{
    if (auto infoRef = resBinder->GetTextureSourceInfo(key); infoRef)
    {
        ChangeBindKeyWithIndex(key, *infoRef, 0);
        return FitToTextureSource();
    }

    return false;
}

void PatchTextureStd3::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
    for (size_t index : views::iota(0u, 3u))
    {
        PatchTextureStd1* tex1 = ComponentCast<PatchTextureStd1*>(GetChildComponent(index));
        tex1->ChangeBindKeyWithIndex(key, sourceInfo, sourceIndex * 3 + index);
    }
}

Rectangle PatchTextureStd3::GetSource() const noexcept
{
    PatchTextureStd1* tex1 = ComponentCast<PatchTextureStd1*>(GetChildComponent(0));
    if (!tex1) return {};

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


