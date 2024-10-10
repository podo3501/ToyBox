#include "pch.h"
#include "ImagePart.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"

ImagePart::ImagePart(const std::wstring& filename, const Rectangle& source) :
	m_filename{ filename },
	m_source{ source }
{}

bool ImagePart::Load(ILoadData* load)
{
	auto result = load->LoadTexture(m_filename, nullptr, m_index, &m_size);
	
	if (!m_source.IsEmpty()) return result;

	m_source = { 0, 0, static_cast<long>(m_size.x), static_cast<long>(m_size.y) };

	return result;
}

bool ImagePart::IsHover(int mouseX, int mouseY) const noexcept
{
	Vector2 mouseLocalPos(static_cast<float>(mouseX) - m_position.x, static_cast<float>(mouseY) - m_position.y);
	return m_destination.Contains(mouseLocalPos);
}

void ImagePart::Render(IRender* render) 
{
	Rectangle destination(m_destination);
	destination.Offset(m_position.x, m_position.y);

	RECT source(m_source);
	render->Render(m_index, destination, &source);
}


