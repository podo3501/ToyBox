#include "pch.h"
#include "PatchTextureLite3.h"
#include "PatchTextureLite1.h"
#include "Utility.h"
#include "../../UIUtility.h"
#include "../../../TextureResourceBinder/TextureResourceBinder.h"
#include "../../../JsonOperation/JsonOperation.h"

PatchTextureLite3::~PatchTextureLite3() = default;
PatchTextureLite3::PatchTextureLite3() = default;
PatchTextureLite3::PatchTextureLite3(DirectionType dirType) noexcept :
	m_dirType{ dirType }
{}

PatchTextureLite3::PatchTextureLite3(const PatchTextureLite3& o) :
	PatchTextureLite{ o },
	m_dirType{ o.m_dirType }
{}

unique_ptr<UIComponent> PatchTextureLite3::CreateClone() const
{
	return unique_ptr<PatchTextureLite3>(new PatchTextureLite3(*this));
}

bool PatchTextureLite3::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTextureLite3* o = static_cast<const PatchTextureLite3*>(&rhs);

	return(tie(m_dirType) == tie(o->m_dirType));
}

bool PatchTextureLite3::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	ReturnIfFalse(sources.size() == 3);

	vector<optional<StateFlag::Type>> stateFlags = GetStateFlagsForDirection(m_dirType);
	for (auto idx : views::iota(0u, sources.size()))
	{
		unique_ptr<PatchTextureLite> pTexL1 = make_unique<PatchTextureLite1>();
		if (auto flag = stateFlags[idx]; flag) pTexL1->SetStateFlag(*flag, true);
		pTexL1->SetupLayout(index, { sources[idx] });
		UIEx(this).AttachComponent(move(pTexL1), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	ChangeSize(size, true);

	return true;
}

void PatchTextureLite3::SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept
{
	ForEachTex([index, &sources](PatchTextureLite1* tex, size_t idx) {
		tex->SetIndexedSource(index, { sources[idx] });
		return true;
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

bool PatchTextureLite3::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEachTex([this, &sizes](PatchTextureLite1* tex1, size_t index) {
		return tex1->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTextureLite3::ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(m_dirType, sizes);
	ReturnIfFalse(ForEachTex([this, &size, &positions](PatchTextureLite1*, size_t index) {
		return ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTextureLite3::ImplementChangeSize(const XMUINT2& size) noexcept
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

bool PatchTextureLite3::FitToTextureSource() noexcept
{
	XMUINT2 totalSize{};
	vector<XMUINT2> sizes{};
	auto result = ForEachTex([&totalSize, &sizes, this](PatchTextureLite1* tex, size_t) {
		ReturnIfFalse(tex->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(tex->GetArea());
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

bool PatchTextureLite3::ForEachTex(predicate<PatchTextureLite1*, size_t> auto&& each)
{
	for (size_t idx : views::iota(0u, 3u))
	{
		PatchTextureLite1* tex = ComponentCast<PatchTextureLite1*>(GetChildComponent(idx));
		if (!each(tex, idx)) return false;
	}
	return true;
}

void PatchTextureLite3::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("DirectionType", m_dirType);
}