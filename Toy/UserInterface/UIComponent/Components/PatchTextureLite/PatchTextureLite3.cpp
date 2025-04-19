#include "pch.h"
#include "PatchTextureLite3.h"
#include "PatchTextureLite1.h"
#include "Utility.h"
#include "../../UIUtility.h"
#include "../../../JsonOperation/JsonOperation.h"

PatchTextureLite3::~PatchTextureLite3() = default;
PatchTextureLite3::PatchTextureLite3() noexcept :
	m_dirType{ DirectionType::Horizontal }
{
	m_impl.SetOwner(this);
}

PatchTextureLite3::PatchTextureLite3(DirectionType dirType) noexcept
{
	m_impl.SetOwner(this);
	SetDirectionType(dirType);
}

PatchTextureLite3::PatchTextureLite3(const PatchTextureLite3& o) :
	PatchTextureLite{ o },
	m_dirType{ o.m_dirType }
{
	m_impl.SetOwner(this);
}

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

void PatchTextureLite3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	m_impl.SetIndexedSource(index, sources, [&sources](size_t idx) {
		return vector<Rectangle>{ sources[idx] };
		});
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [](auto component) {
		PatchTextureLite1* tex = ComponentCast<PatchTextureLite1*>(component);
		return tex->GetSource();
		});
	return srcList;
}

bool PatchTextureLite3::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(m_dirType, size, list));

	return m_impl.ChangeSize(m_dirType, size, components);
}

bool PatchTextureLite3::FitToTextureSource() noexcept
{
	return m_impl.FitToTextureSource(m_dirType, [this](const XMUINT2& size, XMUINT2& totalSize) {
		switch (m_dirType) {
		case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
		case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
		}});
}

Rectangle PatchTextureLite3::GetFirstComponentSource() const noexcept
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