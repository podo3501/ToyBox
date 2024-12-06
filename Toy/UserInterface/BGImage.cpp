#include "pch.h"
#include "BGImage.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "UILayout.h"
#include "ImagePartSet.h"
#include "JsonOperation.h"

using json = nlohmann::json;

BGImage::~BGImage() = default;
BGImage::BGImage()
{}
BGImage::BGImage(const BGImage& other) :
	UIComponent{ other }
{
	m_imagePartSet = make_unique<ImagePartSet>(*other.m_imagePartSet.get());
}

unique_ptr<UIComponent> BGImage::Clone()
{
	auto clone = make_unique<BGImage>(*this);
	clone->SetName(clone->GetName() + "_clone");
	return clone;
}

bool BGImage::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const BGImage* rhs = static_cast<const BGImage*>(&o);
	
	return CompareUniquePtr(m_imagePartSet, rhs->m_imagePartSet);
}

bool BGImage::LoadResources(ILoadData* load)
{ 
	ReturnIfFalse(m_imagePartSet->LoadResources(load));
	ReturnIfFalse(m_imagePartSet->SetDestination(GetArea()));

	return true;
}

void BGImage::SetImage(const string& name, const UILayout& layout, const ImageSource& sources)
{
	SetName(name);
	SetLayout(layout);

	m_imagePartSet = make_unique<ImagePartSet>(sources);
}

bool BGImage::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(m_imagePartSet->SetDestination(area));
	UIComponent::ChangeArea(area);

	return true;
}

bool BGImage::Update(const XMINT2& position, MouseTracker*) noexcept
{
	const XMINT2& pos = GetPositionByLayout(position);
	ReturnIfFalse(m_imagePartSet->SetPosition(pos));	

	return true;
}

void BGImage::Render(IRender* renderer)
{
	m_imagePartSet->Render(renderer, GetSelected());
}

void BGImage::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("ImagePartSet", m_imagePartSet);
}