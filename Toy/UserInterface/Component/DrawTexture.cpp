#include "pch.h"
#include "DrawTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"

DrawTexture::~DrawTexture() { Release(); }
DrawTexture::DrawTexture() :
	m_resourceInfo{ nullptr }
{}

DrawTexture::DrawTexture(const DrawTexture& o) :
	UIComponent{ o },
	m_resourceInfo{ nullptr }
{
	m_index = o.m_index;
}

void DrawTexture::Release() noexcept
{
	if (!m_resourceInfo) return;
	
	m_resourceInfo->ReleaseTexture(m_index);
	m_resourceInfo = nullptr;
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

bool DrawTexture::CreateTexture(IRenderer* renderer, const XMUINT2& size, UIComponent* component)
{
	if (m_gfxOffset && m_resourceInfo)
		Release();
		
	auto resourceInfo = renderer->GetValue();
	ReturnIfFalse(resourceInfo->CreateRenderTexture(size, component, m_index, &m_gfxOffset));

	m_resourceInfo = resourceInfo;
	return true;
}

void DrawTexture::ImplementRender(IRender* render) const
{
	render;
	return;
}

void DrawTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
}