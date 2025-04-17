#include "pch.h"
#include "PatchTextureLite1.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "../../../TextureResourceBinder/TextureResourceBinder.h"

PatchTextureLite1::~PatchTextureLite1() = default;
PatchTextureLite1::PatchTextureLite1() :
	PatchTextureLite{ TextureSlice::One }
{}

PatchTextureLite1::PatchTextureLite1(const PatchTextureLite1& o) :
	PatchTextureLite{ o },
	m_coord{ o.m_coord }
{}

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