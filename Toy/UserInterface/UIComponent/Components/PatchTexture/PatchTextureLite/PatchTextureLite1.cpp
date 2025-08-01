#include "pch.h"
#include "PatchTextureLite1.h"
#include "Utils/CommonUtil.h"

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
	m_coord.SetOwner(this); //�����ɶ� PatchTextureCoord m_coord{ this }; �� �������� �ʱ� ������ ��������� �־��ش�.
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

bool PatchTextureLite1::BindSourceInfo(size_t index, const vector<Rectangle>& sources)
{
	SetIndexedSource(index, sources);
	if (GetSize() == XMUINT2{})
		SetLayout(GetSourceSize(m_coord));

	return true;
}