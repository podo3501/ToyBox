#include "pch.h"
#include "ImagePart.h"
#include "../../Include/IRenderer.h"
#include "Config.h"
#include "JsonOperation.h"

ImagePart::ImagePart() = default;

ImagePart::ImagePart(const std::wstring& filename, const Rectangle& source) :
	m_filename{ filename },
	m_source{ source }
{}
bool ImagePart::operator==(const ImagePart& o) const noexcept
{
	return tie(m_index, m_filename, m_source ) == tie(o.m_index, o.m_filename, o.m_source );
}

string ImagePart::GetType() const { return string(typeid(ImagePart).name()); }

bool ImagePart::Load(ILoadData* load)
{
	XMUINT2 size{};
	auto result = load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, &size);
	if (!m_source.IsEmpty()) return result;

	m_source = { 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };

	return result;
}

bool ImagePart::IsHover(int mouseX, int mouseY) const noexcept
{
	XMINT2 mouseLocalPos(mouseX - m_position.x, mouseY - m_position.y);
	return m_destination.Contains(mouseLocalPos.x, mouseLocalPos.y);
}

void ImagePart::Render(IRender* render, bool selected) 
{
	Rectangle destination(m_destination);
	destination.Offset(m_position.x, m_position.y);

	RECT source(m_source);
	render->Render(m_index, destination, &source, selected);
}

void ImagePart::SerializeIO(JsonOperation& operation)
{
	operation.Process("Index", m_index);
	operation.Process("Filename", m_filename);
	operation.Process("Source", m_source);
}


