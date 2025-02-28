#include "pch.h"
#include "RenderTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"
#include "../../InputManager.h"

RenderTexture::~RenderTexture() { Release(); }
RenderTexture::RenderTexture() :
	m_component{ nullptr },
	m_texController{ nullptr }
{
	SetStateFlag(StateFlag::Render | StateFlag::RenderTexture, true);
}

RenderTexture::RenderTexture(const RenderTexture& o) :
	UIComponent{ o },
	m_component{ nullptr },
	m_texController{ nullptr },
	m_index{ o.m_index },
	m_mouseEvents{ o.m_mouseEvents }
{
	ReloadDatas();
}

void RenderTexture::Release() noexcept
{
	if (m_texController && m_index)
	{
		m_texController->ReleaseTexture(*m_index);
		m_texController = nullptr;
	}
}

void RenderTexture::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_component = componentList[0];
}

unique_ptr<UIComponent> RenderTexture::CreateClone() const
{
	return unique_ptr<RenderTexture>(new RenderTexture(*this));
}

bool RenderTexture::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const RenderTexture* o = static_cast<const RenderTexture*>(&rhs);

	ReturnIfFalse(tie(m_mouseEvents) == tie(o->m_mouseEvents));
	ReturnIfFalse(EqualComponent(m_component, o->m_component));

	return true;
}

bool RenderTexture::ImplementPostLoaded(ITextureController* texController)
{
	if (m_gfxOffset && m_texController)
		Release();
	
	size_t index{ 0 };
	ReturnIfFalse(texController->CreateRenderTexture(m_component, GetSize(), GetPosition(), index, &m_gfxOffset));

	m_index = index;
	m_texController = texController;
	return true;
}

bool RenderTexture::Setup(const UILayout& layout, unique_ptr<UIComponent>&& component) noexcept
{
	SetLayout(layout);
	m_component = component.get();
	UIEx(this).AttachComponent(move(component), {});

	return true;
}

bool RenderTexture::ModifyTexture(const XMUINT2& size)
{
	ReturnIfFalse(m_texController->ModifyRenderTexture(*m_index, size));
	SetSize(size);

	return true;
}

void RenderTexture::CheckMouseInArea() noexcept
{
	m_mouseInArea = Contains(GetArea(), InputManager::GetMouse().GetPosition());
}

void RenderTexture::CheckEnterLeave() noexcept
{
	m_entered = !m_lastMouseInArea && m_mouseInArea;
	m_left = m_lastMouseInArea && !m_mouseInArea;
	m_lastMouseInArea = m_mouseInArea;
}

bool RenderTexture::ImplementActiveUpdate() noexcept
{
	CheckMouseInArea();
	CheckEnterLeave();	
	SetStateFlag(StateFlag::ActiveUpdate, m_mouseInArea && m_mouseEvents);

	return true;
}

bool RenderTexture::ImplementUpdatePosition(const DX::StepTimer&, const XMINT2& position) noexcept
{
	if (IsDirty())
		m_position = GetPositionByLayout(position);

	return true;
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	const auto& size = GetSize();
	Rectangle destination(m_position.x, m_position.y, size.x, size.y);

	RECT source{ 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };
	render->Render(*m_index, destination, &source);

	return;
}

void RenderTexture::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("MouseEvents", m_mouseEvents);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

//RenderStateFlag가 true면 렌더 항목도 RenderTexture에 찍히도록 한다.
unique_ptr<RenderTexture> CreateRenderTexture(const UILayout& layout, unique_ptr<UIComponent>&& component)
{
	unique_ptr<RenderTexture> renderTexture = make_unique<RenderTexture>();
	if (!renderTexture->Setup(layout, move(component))) return nullptr;
	return renderTexture;
}