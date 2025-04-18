#include "pch.h"
#include "PatchTextureLite3.h"
#include "PatchTextureLite1.h"
#include "Utility.h"
#include "../../UIUtility.h"

PatchTextureLite3::~PatchTextureLite3() = default;
PatchTextureLite3::PatchTextureLite3() noexcept
{
	m_impl.SetOwner(this);
}

PatchTextureLite3::PatchTextureLite3(DirectionType dirType) noexcept
{
	m_impl.SetOwner(this);
	m_impl.SetDirectionType(dirType);
}

PatchTextureLite3::PatchTextureLite3(const PatchTextureLite3& o) :
	PatchTextureLite{ o },
	m_impl{ o.m_impl }
{
	m_impl.SetOwner(this);
}

unique_ptr<UIComponent> PatchTextureLite3::CreateClone() const
{
	return unique_ptr<PatchTextureLite3>(new PatchTextureLite3(*this));
}

bool PatchTextureLite3::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTextureLite3* o = static_cast<const PatchTextureLite3*>(&rhs);

	return (tie(m_impl) == tie(o->m_impl));
}

bool PatchTextureLite3::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	ReturnIfFalse(sources.size() == 3);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_impl.GetDirectionType());
	for (auto idx : views::iota(0u, sources.size()))
	{
		unique_ptr<PatchTextureLite> tex = make_unique<PatchTextureLite1>();
		if (auto flag = stateFlags[idx]; flag) tex->SetStateFlag(*flag, true);
		tex->SetupLayout(index, { sources[idx] });
		UIEx(this).AttachComponent(move(tex), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	return ChangeSize(size, true);
}

void PatchTextureLite3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	m_impl.SetIndexedSource(index, sources);
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
	ReturnIfFalse(IsBiggerThanSource(m_impl.GetDirectionType(), size, list));

	return m_impl.ChangeSize(size, components);
}

bool PatchTextureLite3::FitToTextureSource() noexcept
{
	//XMUINT2 totalSize{};
	//vector<XMUINT2> sizes{};
	//auto result = ForEachTex([&totalSize, &sizes, this](PatchTextureLite1* tex, size_t) {
	//	ReturnIfFalse(tex->FitToTextureSource());
	//	const XMUINT2 size = GetSizeFromRectangle(tex->GetArea());
	//	sizes.emplace_back(size);
	//	switch (m_dirType) {
	//	case DirectionType::Horizontal: totalSize.x += size.x; totalSize.y = max(totalSize.y, size.y); break;
	//	case DirectionType::Vertical: totalSize.y += size.y; totalSize.x = max(totalSize.x, size.x); break;
	//	}
	//	return true;
	//	});
	//ReturnIfFalse(result); //result 변수를 써서 ReturnIfFalse를 한 이유는 람다에 디버깅(브레이크 포인터)이 안되기 때문이다.
	//ReturnIfFalse(ApplyPositions(totalSize, sizes));

	//SetSize(totalSize);
	//return true;

	return m_impl.FitToTextureSource();
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
	m_impl.SerializeIO(operation);
}