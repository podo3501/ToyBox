#include "pch.h"
#include "DrawTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"

DrawTexture::~DrawTexture()
{
	if (m_resourceInfo)
	{
		m_resourceInfo->ReleaseTexture(m_index);
		m_resourceInfo = nullptr;
	}
}

DrawTexture::DrawTexture() :
	m_resourceInfo{ nullptr }
{
	
}

DrawTexture::DrawTexture(const DrawTexture& o) :
	UIComponent{ o },
	m_resourceInfo{ nullptr }
{
	m_index = o.m_index;
}

unique_ptr<UIComponent> DrawTexture::CreateClone() const
{
	return unique_ptr<DrawTexture>(new DrawTexture(*this));
}

bool DrawTexture::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const DrawTexture* o = static_cast<const DrawTexture*>(&rhs);

	auto result = tie(m_index) == tie(o->m_index);
	assert(result);

	return result;
}

bool DrawTexture::CreateTexture(IGetValue* resourceInfo, const XMUINT2& size, UIComponent* component, ImTextureID* texID)
{
	m_resourceInfo = resourceInfo;
	return true;
}

bool DrawTexture::ImplementUpdatePosition(const XMINT2& position) noexcept
{
	return true;
}

void DrawTexture::ImplementRender(IRender* render) const
{

}

void DrawTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
}