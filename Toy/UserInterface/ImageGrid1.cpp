#include "pch.h"
#include "ImageGrid1.h"
#include "../../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"
#include "../Config.h"
#include "UIType.h"
#include "JsonOperation.h"

ImageGrid1::~ImageGrid1() = default;
ImageGrid1::ImageGrid1() = default;

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
	return load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, nullptr);
}

bool ImageGrid1::Update(const XMINT2& position, InputManager*) noexcept
{
	m_position = GetPositionByLayout(position);

	return true;
}

void ImageGrid1::Render(IRender* render)
{
	Rectangle destination(m_destination);
	destination.Offset(m_position.x, m_position.y);

	RECT source(m_source);
	render->Render(m_index, destination, &source, GetSelected());
}

bool ImageGrid1::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 1) return false;

	SetName(name);
	SetLayout(layout);

	m_filename = source.filename;
	m_source = source.list.at(0);
	m_destination = layout.GetArea();

	return true;
}

void ImageGrid1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
	operation.Process("Filename", m_filename);
	operation.Process("Source", m_source);
}

unique_ptr<ImageGrid1> CreateImageGrid1(const string& name, const UILayout& layout, const ImageSource& source)
{
	auto imgGrid1 = make_unique<ImageGrid1>();
	if(!imgGrid1->SetImage(name, layout, source)) return nullptr;
	
	return imgGrid1;
}