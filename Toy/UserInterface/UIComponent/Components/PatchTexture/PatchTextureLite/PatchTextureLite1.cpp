#include "pch.h"
#include "PatchTextureLite1.h"
#include "Utility.h"

PatchTextureLite1::~PatchTextureLite1() = default;
PatchTextureLite1::PatchTextureLite1() :
	PatchTextureLite{ TextureSlice::One }
{
	m_coord.SetOwner(this);
}

PatchTextureLite1::PatchTextureLite1(const PatchTextureLite1& o) :
	PatchTextureLite{ o },
	m_coord{ o.m_coord }
{
	m_coord.SetOwner(this); //생성될때 PatchTextureCoord m_coord{ this }; 를 실행하지 않기 때문에 명시적으로 넣어준다.
}

unique_ptr<UIComponent> PatchTextureLite1::CreateClone() const
{
	return unique_ptr<PatchTextureLite1>(new PatchTextureLite1(*this));
}

bool PatchTextureLite1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTextureLite1* o = static_cast<const PatchTextureLite1*>(&rhs);

	return (tie(m_coord) == tie(o->m_coord));
}

bool PatchTextureLite1::Setup(const XMUINT2& size)
{
	SetLayout(size);
	return true;
}

bool PatchTextureLite1::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	SetIndexedSource(index, sources);
	const XMUINT2& curSize = (size == XMUINT2{}) ? GetSourceSize(m_coord) : size;
	SetLayout(curSize);

	return true;
}

bool PatchTextureLite1::BindSourceInfo(size_t index, const vector<Rectangle>& sources)
{
	SetIndexedSource(index, sources);
	if (GetSize() == XMUINT2{})
		SetLayout(GetSourceSize(m_coord));

	return true;
}

unique_ptr<PatchTextureLite1> CreatePatchTextureLite1(const XMUINT2& size)
{
	return CreateIfSetup(move(make_unique<PatchTextureLite1>()), size); //?!? 아마 CreateIfSetup 이거 템플릿으로 바꿀 수 있을듯
}