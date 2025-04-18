#include "pch.h"
#include "PatchTexture3.h"
#include "PatchTexture1.h"
#include "Utility.h"
#include "../../UIUtility.h"

PatchTexture3::~PatchTexture3() = default;
PatchTexture3::PatchTexture3() noexcept
{
    m_impl.SetOwner(this);
}

PatchTexture3::PatchTexture3(const PatchTexture3& o) :
    PatchTexture{ o },
    m_impl{ o.m_impl }
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

    return (tie(m_impl) == tie(o->m_impl));
}

void PatchTexture3::SetDirectionType(DirectionType dirType) noexcept
{
    m_impl.SetDirectionType(dirType);
    SetTextureSlice(DirTypeToTextureSlice(dirType));
}

bool PatchTexture3::Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept
{
    SetLayout({ layout.GetSize(), Origin::LeftTop });
    SetDirectionType(dirType);

    vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_impl.GetDirectionType());
    for (size_t idx : views::iota(0, 3)) //bindKey�� ��ȸ�� �� ���� �� ������ �ڽĵ��� �����Ѵ�.
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
    if (GetSize() == XMUINT2{}) //PatchTexture9�� ����� �ʱ� ũ�Ⱚ�� 0�� ���� �� �ִ�.
        SetSize(UIEx(this).GetChildrenBoundsSize());
    else
        ChangeSize(GetSize(), true);
    UpdatePositionsManually(); //?!? �̰� �ʿ��ұ� �𸣰ڳ�

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
    m_impl.SetIndexedSource(index, sources);
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
    ReturnIfFalse(IsBiggerThanSource(m_impl.GetDirectionType(), size, list));

    return m_impl.ChangeSize(size, components);
}

bool PatchTexture3::FitToTextureSource() noexcept
{
    //XMUINT2 totalSize{};
    //vector<XMUINT2> sizes{};
    //auto result = ForEachPatchTexture1([&totalSize, &sizes, this](PatchTexture1* tex1, size_t) {
    //    ReturnIfFalse(tex1->FitToTextureSource());
    //    const XMUINT2 size = GetSizeFromRectangle(tex1->GetArea());
    //    sizes.emplace_back(size);
    //    switch (m_dirType) {
    //    case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
    //    case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
    //    }
    //    return true;
    //    });
    //ReturnIfFalse(result); //result ������ �Ἥ ReturnIfFalse�� �� ������ ���ٿ� �����(�극��ũ ������)�� �ȵǱ� �����̴�.
    //ReturnIfFalse(ApplyPositions(totalSize, sizes));

    //SetSize(totalSize);
    //return true;

    return m_impl.FitToTextureSource();
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
    m_impl.SerializeIO(operation);
}

unique_ptr<PatchTexture3> CreatePatchTexture3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex)
{
    auto patchTex3 = make_unique<PatchTexture3>();
    return CreateIfSetup(move(patchTex3), layout, dirType, bindKey, sourceIndex);
}


