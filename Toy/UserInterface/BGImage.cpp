#include "pch.h"
#include "BGImage.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"
#include "JsonHelper.h"

using json = nlohmann::json;

BGImage::~BGImage() = default;
BGImage::BGImage()
{}
BGImage::BGImage(const BGImage& other)
	: IRenderItem{ other }
{
	m_position = other.m_position;
	m_imagePartSet = make_unique<ImagePartSet>(*other.m_imagePartSet.get());
}

bool BGImage::LoadResources(ILoadData* load)
{ 
	ReturnIfFalse(m_imagePartSet->LoadResources(load));
	ReturnIfFalse(m_imagePartSet->SetDestination(GetArea()));

	return true;
}

bool BGImage::ReadProperty(const nlohmann::json& data)
{
	tie(m_imagePartSet, m_position) = GetProperty<ImagePartSet>(data);
	ReturnIfNullptr(m_imagePartSet);

	return true;
}

void BGImage::SetImage(const string& name, const Vector2 position, const UILayout& layout, const ImageSource& sources)
{
	SetName(name);
	SetLayout(layout);

	m_position = position;
	m_imagePartSet = make_unique<ImagePartSet>(sources);
}

bool BGImage::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(m_imagePartSet->SetDestination(area));
	IRenderItem::ChangeArea(area);

	return true;
}

bool BGImage::Update(const Vector2& position, const Mouse::ButtonStateTracker*) noexcept
{
	auto layout = GetLayout();
	const Vector2 pos = layout->GetPosition(m_position + position);

	ReturnIfFalse(m_imagePartSet->Update(pos));	//Update보다 SetPosition으로 바꾸는게 더 직관적일듯

	return true;
}

void BGImage::Render(IRender* renderer)
{
	m_imagePartSet->Render(renderer);
}

unique_ptr<IRenderItem> BGImage::Clone()
{
	return make_unique<BGImage>(*this);
}

void BGImage::SetPosition(const Vector2& pos) noexcept
{
	m_position = pos;
}