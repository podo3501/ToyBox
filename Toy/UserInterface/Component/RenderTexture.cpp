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
	//같은 라인에서 RenderTexture 옵션이 있는 애가 있을 것이다.
	UIComponent* component = this->GetSiblingComponent(StateFlag::RenderTexture);
	if (!component) return;
	
	m_component = component;
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
	
	ReturnIfFalse(texController->CreateRenderTexture(m_component, GetSize(), GetPosition(), m_index, &m_gfxOffset));
	if (m_component->HasStateFlag(StateFlag::RenderTexture))
	{
		//m_component->ChangeOrigin(Origin::LeftTop);
	}

	m_texController = texController;
	return true;
}

bool RenderTexture::Setup(const UILayout& layout, bool renderTextureOnly, UIComponent* component)
{
	SetLayout(layout);
	if (renderTextureOnly) component->SetStateFlag(StateFlag::RenderTexture, true);
	m_component = component;
	
	return true;
}

bool RenderTexture::ModifyTexture(const XMUINT2& size)
{
	return m_texController->ModifyRenderTexture(m_index, size);
}

bool RenderTexture::ImplementUpdatePosition(const DX::StepTimer&, const XMINT2& position) noexcept
{
	if (IsDirty())
		m_position = position;

	//여기서 휠을 움직이면 m_component의 위치를 조정한다.

	return true;
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	const auto& size = GetSize();
	Rectangle destination(m_position.x, m_position.y, size.x, size.y);

	RECT source{ 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };
	render->Render(m_index, destination, &source);

	return;
}

void RenderTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

//RenderStateFlag가 true면 렌더 항목도 RenderTexture에 찍히도록 한다.
unique_ptr<RenderTexture> CreateRenderTexture(const UILayout& layout, bool renderTextureOnly, UIComponent* component)
{
	unique_ptr<RenderTexture> renderTexture = make_unique<RenderTexture>();
	if (!renderTexture->Setup(layout, renderTextureOnly, component)) return nullptr;
	return renderTexture;
}