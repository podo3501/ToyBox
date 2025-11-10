#include "pch.h"
#include "RenderTexture.h"
#include "IRenderer.h"
#include "Locator/InputLocator.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/SerializerIO/SerializerIO.h"

RenderTexture::~RenderTexture() 
{ 
	Release(); 
}

RenderTexture::RenderTexture() :
	m_component{ nullptr },
	m_texController{ nullptr }
{}

RenderTexture::RenderTexture(const RenderTexture& o) :
	UIComponent{ o },
	m_component{ nullptr },
	m_texController{ o.m_texController },
	m_index{ o.m_index }
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
	vector<UIComponent*> componentList = GetChildren();
	m_component = componentList[0];
}

bool RenderTexture::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const RenderTexture* o = static_cast<const RenderTexture*>(&rhs);
	ReturnIfFalse(EqualComponent(m_component, o->m_component));

	return true;
}

void RenderTexture::ImplementPositionUpdated() noexcept
{
	if (m_texController && m_index)
		m_texController->ModifyRenderTexturePosition(*m_index, GetLeftTop());
}

//?!? rendertexture를 등록하는 것을 bind를 통해서 넣으면 텍스쳐 관리를 한군데에 몰아서 할 수 있을꺼 같은데.
bool RenderTexture::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController* texController) noexcept
{
	Release(); //데이터가 존재하면 지운다.

	if (GetSize() == XMUINT2{})
		SetSize(UIEx(this).GetChildrenBoundsSize());
	
	size_t index{ 0 };
	UpdatePositionsManually(true);
	ReturnIfFalse(texController->CreateRenderTexture(m_component, GetArea(), index, &m_gfxOffset));

	m_index = index;
	m_texController = texController;

	return true;
}

bool RenderTexture::ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept
{
	ReturnIfFalse(m_component->ChangeSize(size, isForce));
	ReturnIfFalse(m_texController->ModifyRenderTextureSize(*m_index, size));

	return true;
}

bool RenderTexture::Setup(const UILayout& layout, unique_ptr<UIComponent> component) noexcept
{
	SetLayout(layout);
	m_component = component.get();
	UIEx(this).AttachComponent(move(component), {});

	return true;
}

bool RenderTexture::Setup(unique_ptr<UIComponent> component) noexcept
{
	UILayout layout{ component->GetSize() };
	return Setup(layout, move(component));
}

void RenderTexture::ImplementRender(ITextureRender* render) const
{
	if (m_texController && m_index)
		m_texController->ModifyRenderTexturePosition(*m_index, GetLeftTop()); //?!? 좌표가 바뀌면 RenderTexture 안에 좌표가 갱신이 되지 않아서 이상해진다. update에 넣고 싶은데 툴에서는 update가 돌지 않는다.

	const auto& size = GetSize();
	RECT source{ 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) };
	render->Render(m_index.value(), GetArea(), &source);

	return;
}

void RenderTexture::ProcessIO(SerializerIO& serializer)
{
	UIComponent::ProcessIO(serializer);

	if (serializer.IsWrite()) return;
	ReloadDatas();
}