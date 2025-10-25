#include "pch.h"
#include "PatchTextureCoord.h"
#include "IRenderer.h"
#include "UserInterface/UIComponent/UIComponent.h"
#include "Shared/Utils/GeometryExt.h"

PatchTextureCoord::~PatchTextureCoord() = default;
PatchTextureCoord::PatchTextureCoord() noexcept :
	m_component{ nullptr }
{}

PatchTextureCoord::PatchTextureCoord(const PatchTextureCoord& o) noexcept :
	m_component{ nullptr },
	m_index{ o.m_index },
	m_source{ o.m_source }
{}

bool PatchTextureCoord::operator==(const PatchTextureCoord& rhs) const noexcept
{
	auto result = (tie(m_index, m_source) == tie(rhs.m_index, rhs.m_source));
	Assert(result);

	return result;
}

void PatchTextureCoord::Render(ITextureRender* render) const
{
	RECT source = RectangleToRect(m_source);
	RECT dest = RectangleToRect(m_component->GetArea());

	render->Render(*m_index, dest, &source);
}

void PatchTextureCoord::SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept
{
	m_index = index;
	m_source = source[0];
}

bool PatchTextureCoord::FitToTextureSource() noexcept
{
	Assert(!m_source.IsEmpty()); //��ǥ�� ���°ǵ� �Ƹ� �ε��ϰ��� Bind�� ���� ��ų� ������ ������ų� attach�� �ȵż� bind�� �ȵ��� ���ɼ��� �ִ�.
	if (m_source.IsEmpty()) return false;

	m_component->SetSize(GetSizeFromRectangle(m_source));
	return true;
}

//utility
XMUINT2 GetSourceSize(const PatchTextureCoord& coord) noexcept
{
	return GetSizeFromRectangle(coord.GetSource());
}
