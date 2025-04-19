#include "pch.h"
#include "PatchTextureLite1.h"

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

bool PatchTextureLite1::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	SetIndexedSource(index, sources);
	const XMUINT2& curSize = (size == XMUINT2{}) ? GetSourceSize(m_coord) : size;
	SetLayout({ curSize, Origin::LeftTop });

	return true;
}