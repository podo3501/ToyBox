#include "pch.h"
#include "ImagePart.h"
#include "../Include/IRenderer.h"
#include "Utility.h"

ImagePart::ImagePart(const std::wstring& filename) :
	m_filename{ filename }
{}

bool ImagePart::Load(ILoadData* load)
{
	return load->LoadTexture(m_filename, nullptr, m_index, &m_size);
}

void ImagePart::MakeLocalDestination(const Vector2& origin)
{
	m_localDestination.x = static_cast<long>(m_localPosition.x - origin.x);
	m_localDestination.y = static_cast<long>(m_localPosition.y - origin.y);
	m_localDestination.width = m_size.x;
	m_localDestination.height = m_size.y;
}

void ImagePart::Render(IRender* render)
{
	Rectangle destination(m_localDestination);
	destination.Offset(static_cast<long>(m_position.x), static_cast<long>(m_position.y));
	render->Render(m_index, destination, nullptr);
}


