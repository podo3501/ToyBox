#include "pch.h"
#include "UIModuleAsComponent.h"
#include "../../UIModul2.h"
#include "Panel.h"

UIModuleAsComponent::~UIModuleAsComponent() = default;
UIModuleAsComponent::UIModuleAsComponent() = default;

unique_ptr<UIComponent> UIModuleAsComponent::CreateClone() const { return nullptr; }

bool UIModuleAsComponent::Setup(UIModul2* modul2) noexcept
{
	auto mainPanel = modul2->GetMainPanel();
	SetSize(mainPanel->GetSize());

	m_modul2 = modul2;
	return true;
}

bool UIModuleAsComponent::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	return m_modul2->BindTextureResources();
}

bool UIModuleAsComponent::ImplementUpdate(const DX::StepTimer& timer) noexcept 
{ 
	return m_modul2->Update(timer);
}

void UIModuleAsComponent::ImplementRender(ITextureRender* render) const
{
	m_modul2->Render(render);
}
