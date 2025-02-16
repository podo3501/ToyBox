#include "pch.h"
#include "RenderTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"

RenderTexture::~RenderTexture() { Release(); }
RenderTexture::RenderTexture() :
	m_texController{ nullptr }
{}

RenderTexture::RenderTexture(const RenderTexture& o) :
	UIComponent{ o },
	m_texController{ nullptr }
{
	m_index = o.m_index;
}

void RenderTexture::Release() noexcept
{
	if (!m_texController) return;
	
	m_texController->ReleaseTexture(m_index);
	m_texController = nullptr;
}

unique_ptr<UIComponent> RenderTexture::CreateClone() const
{
	return unique_ptr<RenderTexture>(new RenderTexture(*this));
}

bool RenderTexture::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const RenderTexture* o = static_cast<const RenderTexture*>(&rhs);

	auto result = tie(m_index) == tie(o->m_index);
	assert(result);

	return result;
}

bool RenderTexture::CreateTexture(IRenderer* renderer, const XMUINT2& size, UIComponent* component)
{
	if (m_gfxOffset && m_texController)
		Release();
		
	auto texController = renderer->GetTextureController();
	ReturnIfFalse(texController->CreateRenderTexture(size, component, m_index, &m_gfxOffset));

	m_texController = texController;
	return true;
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	render;
	return;
}

void RenderTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Index", m_index);
}