#include "pch.h"
#include "PatchTextureCoord.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIComponent/UIComponent.h"
#include "Utility.h"

PatchTextureCoord::~PatchTextureCoord() = default;
PatchTextureCoord::PatchTextureCoord(UIComponent* component) noexcept :
	m_component{ component }
{}

PatchTextureCoord::PatchTextureCoord(const PatchTextureCoord& o) noexcept :
	m_index{ o.m_index },
	m_source{ o.m_source }
{}

void PatchTextureCoord::Render(ITextureRender* render) const
{
	const auto& position = m_component->GetPosition();
	const auto& size = m_component->GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(*m_index, destination, &source);
}

void PatchTextureCoord::SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept
{
	m_index = index;
	m_source = source[0];
}

bool PatchTextureCoord::FitToTextureSource() noexcept
{
	if (m_source.IsEmpty()) return false;

	m_component->SetSize(GetSizeFromRectangle(m_source));
	return true;
}

//utility
XMUINT2 GetSourceSize(const PatchTextureCoord& coord) noexcept
{
	return GetSizeFromRectangle(coord.GetSource());
}
