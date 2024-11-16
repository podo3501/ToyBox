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
	: UIComponent{ other }
{
	m_imagePartSet = make_unique<ImagePartSet>(*other.m_imagePartSet.get());
}

unique_ptr<UIComponent> BGImage::Clone()
{
	auto clone = make_unique<BGImage>(*this);
	clone->SetName(clone->GetName() + "_clone");
	return clone;
}

//bool BGImage::ReadProperty(const nlohmann::json& data)
//{
//	ImageSource imgSource;
//	ReturnIfFalse(imgSource.Read(data));
//	m_imagePartSet = make_unique<ImagePartSet>(imgSource);
//	
//	//auto imagePartSet = make_unique<ImagePartSet>();
//	//m_imagePartSet = CreateProperty<ImagePartSet>(data);
//	//ReturnIfNullptr(m_imagePartSet);
//
//	return true;
//}

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

bool BGImage::Update(const Vector2& position, const Mouse::ButtonStateTracker*) noexcept
{
	auto layout = GetLayout();
	const Vector2 pos = layout->GetPosition(position);

	ReturnIfFalse(m_imagePartSet->Update(pos));	//Update보다 SetPosition으로 바꾸는게 더 직관적일듯

	return true;
}

void BGImage::Render(IRender* renderer)
{
	m_imagePartSet->Render(renderer);
}