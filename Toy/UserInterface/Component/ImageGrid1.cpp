#include "pch.h"
#include "ImageGrid1.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../../Config.h"
#include "../JsonOperation.h"

ImageGrid1::~ImageGrid1() = default;
ImageGrid1::ImageGrid1() = default;

ImageGrid1::ImageGrid1(const ImageGrid1& o) :
	UIComponent{ o }
{
	m_index = o.m_index;
	m_filename = o.m_filename;
	m_source = o.m_source;
}

unique_ptr<UIComponent> ImageGrid1::CreateClone() const
{
	return unique_ptr<ImageGrid1>(new ImageGrid1(*this));
}

bool ImageGrid1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageGrid1* o = static_cast<const ImageGrid1*>(&rhs);

	auto result = tie(m_index, m_filename, m_source) == tie(o->m_index, o->m_filename, o->m_source);
	assert(result);

	return result;
}

bool ImageGrid1::LoadResources(ILoadData* load)
{
	ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, nullptr));

	return true;
}

bool ImageGrid1::ImplementUpdate(const XMINT2& position) noexcept
{
	if(IsDirty())
		m_position = GetPositionByLayout(position);

	return true;
}

void ImageGrid1::ImplementRender(IRender* render) const
{
	const auto& size = GetSize();
	Rectangle destination(m_position.x, m_position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(m_index, destination, &source);
}

bool ImageGrid1::SetImage(const UILayout& layout, const ImageSource& source) noexcept
{
	if (source.filename.empty()) return false;
	if (source.list.size() != 1) return false;

	SetLayout(layout);

	m_filename = source.filename;
	m_source = source.list.at(0);

	return true;
}

void ImageGrid1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
	operation.Process("Filename", m_filename);
	operation.Process("Source", m_source);
}