#include "pch.h"
#include "UIModuleAsComponent.h"
#include "../../UIModule.h"

UIModuleAsComponent::~UIModuleAsComponent() = default;
UIModuleAsComponent::UIModuleAsComponent() = default;

unique_ptr<UIComponent> UIModuleAsComponent::CreateClone() const { return nullptr; }

bool UIModuleAsComponent::Setup(unique_ptr<UIModule> module) noexcept
{
	m_module = move(module);
	return true;
}

bool UIModuleAsComponent::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	return m_module->BindTextureResources();
}

const vector<unique_ptr<UIComponent>>& UIModuleAsComponent::GetChildren() const noexcept
{
	return m_module->GetChildren();
}