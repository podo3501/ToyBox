#include "pch.h"
#include "RenderTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"

RenderTexture::~RenderTexture() { Release(); }
RenderTexture::RenderTexture() :
	m_component{ nullptr },
	m_texController{ nullptr }
{}

RenderTexture::RenderTexture(const RenderTexture& o) :
	UIComponent{ o },
	m_component{ nullptr },
	m_texController{ nullptr }
{
	m_index = o.m_index;
	ReloadDatas();
}

void RenderTexture::Release() noexcept
{
	if (!m_texController) return;

	m_texController->ReleaseTexture(m_index);
	m_texController = nullptr;
}

void RenderTexture::ReloadDatas() noexcept
{
	//���� ���ο��� RenderTexture �ɼ��� �ִ� �ְ� ���� ���̴�.
	//RenderTexture �ɼ��� attach detach �Ӽ��� ���� ��� | ������ �ؼ� �������� �Ӽ��� ���ÿ� ���� �� �ֵ��� ����.
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

bool RenderTexture::ImplementPostLoaded(ITextureController* texController)
{
	if (m_gfxOffset && m_texController)
		Release();

	ReturnIfFalse(texController->CreateRenderTexture(GetSize(), m_component, m_index, &m_gfxOffset));

	m_texController = texController;
	return true;
}

bool RenderTexture::Setup(const UILayout& layout, UIComponent* component)
{
	SetLayout(layout);

	m_component = component;
	//ũ�� ���� ���߰ų� ��Ÿ���

	return true;
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

bool RenderTexture::ModifyTexture(const XMUINT2& size)
{
	return m_texController->ModifyRenderTexture(m_index, size);
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	render;
	return;
}

void RenderTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<RenderTexture> CreateRenderTexture(const UILayout& layout, UIComponent* component)
{
	unique_ptr<RenderTexture> renderTexture = make_unique<RenderTexture>();
	if (!renderTexture->Setup(layout, component)) return nullptr;
	return renderTexture;
}