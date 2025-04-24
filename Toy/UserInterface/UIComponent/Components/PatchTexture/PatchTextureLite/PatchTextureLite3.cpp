#include "pch.h"
#include "PatchTextureLite3.h"
#include "PatchTextureLite1.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/JsonOperation/JsonOperation.h"

PatchTextureLite3::~PatchTextureLite3() = default;
PatchTextureLite3::PatchTextureLite3() noexcept :
	PatchTextureLite{ TextureSlice::ThreeH },
	m_dirType{ DirectionType::Horizontal }
{}

PatchTextureLite3::PatchTextureLite3(DirectionType dirType) noexcept
{
	SetDirectionType(dirType);
}

PatchTextureLite3::PatchTextureLite3(const PatchTextureLite3& o) :
	PatchTextureLite{ o },
	m_dirType{ o.m_dirType }
{}

void PatchTextureLite3::SetDirectionType(DirectionType dirType) noexcept
{
	m_dirType = dirType;
	SetTextureSlice(DirTypeToTextureSlice(dirType));
}

unique_ptr<UIComponent> PatchTextureLite3::CreateClone() const
{
	return unique_ptr<PatchTextureLite3>(new PatchTextureLite3(*this));
}

bool PatchTextureLite3::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTextureLite3* o = static_cast<const PatchTextureLite3*>(&rhs);

	return (tie(m_dirType) == tie(o->m_dirType));
}

bool PatchTextureLite3::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	ReturnIfFalse(sources.size() == 3);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
	for (auto idx : views::iota(0u, sources.size()))
	{
		unique_ptr<PatchTextureLite> tex = make_unique<PatchTextureLite1>();
		if (auto flag = stateFlags[idx]; flag) tex->SetStateFlag(*flag, true);
		tex->SetupLayout(index, { sources[idx] });
		UIEx(this).AttachComponent(move(tex), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	if (size == XMUINT2{}) //PatchTexture9을 만들면 초기 크기값이 0로 설정 돼 있다.
		SetLayout({ UIEx(this).GetChildrenBoundsSize(), Origin::LeftTop });
	else
		return ChangeSize(size, true);

	return true;
}

Rectangle PatchTextureLite3::GetSource() const noexcept
{
	PatchTextureLite1* tex1 = ComponentCast<PatchTextureLite1*>(GetChildComponent(0));
	if (!tex1) return {};

	return tex1->GetSource();
}

void PatchTextureLite3::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("DirectionType", m_dirType);

	if (operation.IsWrite()) return;
	SetTextureSlice(DirTypeToTextureSlice(m_dirType));
}