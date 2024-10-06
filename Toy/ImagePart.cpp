#include "pch.h"
#include "ImagePart.h"
#include "../Include/IRenderer.h"

ImagePart::ImagePart(const std::wstring& filename) :
	m_filename{ filename }
{}

bool ImagePart::Load(ILoadData* load)
{
	return load->LoadTexture(m_filename, nullptr, m_index, &m_size);
}

void ImagePart::Render(IRender* render, const Vector2& origin)
{
	render->Render(m_index, m_size, m_localPosition + m_position, origin);
}

