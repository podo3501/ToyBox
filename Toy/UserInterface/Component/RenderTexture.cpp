#include "pch.h"
#include "RenderTexture.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../JsonOperation.h"
#include "../../InputManager.h"

RenderTexture::~RenderTexture() 
{ 
	Release(); 
}

RenderTexture::RenderTexture() :
	m_component{ nullptr },
	m_texController{ nullptr }
{
	SetStateFlag(StateFlag::Render | StateFlag::RenderTexture, true);
}

RenderTexture::RenderTexture(const RenderTexture& o) :
	UIComponent{ o },
	m_component{ nullptr },
	m_texController{ o.m_texController },
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
		m_gfxOffset = {};
		m_index = nullopt;
	}
}

void RenderTexture::AddRef() const noexcept
{
	if (m_texController && m_index)
		m_texController->AddRef(*m_index);
}

unique_ptr<UIComponent> RenderTexture::CreateClone() const
{
	auto clone = unique_ptr<RenderTexture>(new RenderTexture(*this));
	clone->AddRef();
	return clone;
}

void RenderTexture::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_component = componentList[0];
}

bool RenderTexture::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const RenderTexture* o = static_cast<const RenderTexture*>(&rhs);

	ReturnIfFalse(tie(m_mouseEvents) == tie(o->m_mouseEvents));
	ReturnIfFalse(EqualComponent(m_component, o->m_component));

	return true;
}

void RenderTexture::OnAttached(UIComponent*)
{
	UpdatePositionsManually(true);
	if (m_texController && m_index)
		m_texController->ModifyRenderTexturePosition(*m_index, GetPosition());
}

bool RenderTexture::ImplementPostLoaded(ITextureController* texController)
{
	Release(); //데이터가 존재하면 지운다.

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
	ReturnIfFalse(m_texController->ModifyRenderTextureSize(*m_index, size));
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

bool RenderTexture::ImplementUpdate(const DX::StepTimer&) noexcept
{
	CheckMouseInArea();
	CheckEnterLeave();	
	SetChildrenStateFlag(StateFlag::ActiveUpdate, m_mouseInArea && m_mouseEvents);

	return true;
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	const auto& position = GetPosition();
	const auto& size = GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source{ 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };
	render->Render(m_index.value(), destination, &source);

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