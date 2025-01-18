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
	Filename = o.Filename;
	Source = o.Source;
}

unique_ptr<UIComponent> ImageGrid1::CreateClone() const
{
	return unique_ptr<ImageGrid1>(new ImageGrid1(*this));
}

bool ImageGrid1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageGrid1* o = static_cast<const ImageGrid1*>(&rhs);

	auto result = tie(m_index, Filename, Source) == tie(o->m_index, o->Filename, o->Source);
	assert(result);

	return result;
}

bool ImageGrid1::LoadResources(ILoadData* load)
{
	if (!Filename.IsDirty()) return true;
	
	ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(Filename), nullptr, m_index, nullptr));
	Filename.ClearDirty();

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

	RECT source(Rectangle(Source.Get()));
	render->Render(m_index, destination, &source);
}

bool ImageGrid1::SetImage(const UILayout& layout, const ImageSource& source) noexcept
{
	if (source.filename.empty()) return false;
	if (source.list.size() != 1) return false;

	SetLayout(layout);

	Filename = source.filename;
	Source = source.list.at(0);

	return true;
}

void ImageGrid1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
	operation.Process("Filename", Filename);
	operation.Process("Source", Source);
}