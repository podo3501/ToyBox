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
	m_index{ o.m_index },
	m_source{ o.m_source }
{}

unique_ptr<UIComponent> PatchTextureLite1::CreateClone() const
{
	return unique_ptr<PatchTextureLite1>(new PatchTextureLite1(*this));
}

bool PatchTextureLite1::SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size)
{
	SetIndexedSource(index, sources);
	const XMUINT2& curSize = (size == XMUINT2{}) ? GetSizeFromRectangle(m_source) : size; //사이즈가 없다면 source 사이즈로 초기화 한다.
	SetLayout({ curSize, Origin::LeftTop });

	return true;
}

void PatchTextureLite1::ImplementRender(ITextureRender* render) const
{
	const auto& position = GetPosition();
	const auto& size = GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(*m_index, destination, &source);
}

bool PatchTextureLite1::FitToTextureSource() noexcept
{
	if (m_source.IsEmpty()) return false;

	SetSize(GetSizeFromRectangle(m_source));
	return true;
}

void PatchTextureLite1::SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept
{
	m_index = index;
	m_source = source[0];
}
