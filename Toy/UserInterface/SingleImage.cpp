#include "pch.h"
#include "SingleImage.h"
#include "UILayout.h"
#include "../Utility.h"
#include "../Toy/UserInterface/ImagePart.h"
#include "../Toy/UserInterface/JsonOperation.h"

SingleImage::~SingleImage() = default;
SingleImage::SingleImage() = default;

SingleImage::SingleImage(const SingleImage& other) :
	UIComponent{ other }
{
	m_imagePart = make_unique<ImagePart>(*other.m_imagePart.get());
}

bool SingleImage::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const SingleImage* rhs = static_cast<const SingleImage*>(&o);

	return CompareUniquePtr(m_imagePart, rhs->m_imagePart);
}

bool SingleImage::LoadResources(ILoadData* load)
{
	bool result = m_imagePart->Load(load);
	m_imagePart->SyncSourceDest();
	return result;
}

bool SingleImage::Update(const XMINT2& position, InputManager*) noexcept
{
	const XMINT2& pos = GetPositionByLayout(position);
	m_imagePart->SetPosition(pos);

	return true;
}

void SingleImage::Render(IRender* render)
{
	m_imagePart->Render(render, GetSelected());
}

void SingleImage::SetImage(const string& name, const UILayout& layout, const wstring& filename)
{
	SetName(name);
	SetLayout(layout);

	m_imagePart = make_unique<ImagePart>(filename, layout.GetArea());
}

void SingleImage::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("ImagePart", m_imagePart);
}
