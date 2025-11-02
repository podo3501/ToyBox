#include "pch.h"
#include "UIModuleAsComponent.h"
#include "../../UIModule.h"

UIModuleAsComponent::~UIModuleAsComponent() = default;
UIModuleAsComponent::UIModuleAsComponent() = default;

unique_ptr<UIComponent> UIModuleAsComponent::CreateClone() const { return nullptr; }

bool UIModuleAsComponent::Setup(unique_ptr<UIModule> module) noexcept
{
	auto mainPanel = module->GetMainPanel();
	SetSize(mainPanel->GetSize());

	m_module = move(module);
	return true;
}

bool UIModuleAsComponent::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	return m_module->BindTextureResources();
}

bool UIModuleAsComponent::ImplementUpdate(const DX::StepTimer& timer) noexcept 
{ 
	return m_module->Update(timer);
}

void UIModuleAsComponent::ImplementRender(ITextureRender* render) const
{
	m_module->Render(render);
}

const vector<unique_ptr<UIComponent>>& UIModuleAsComponent::GetChildren() const noexcept
{
	return m_module->GetChildren();
}

