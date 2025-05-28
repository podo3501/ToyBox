#include "pch.h"
#include "UIModule.h"
#include "../Include/IRenderer.h"
#include "UIComponent/UIComponent.h"
#include "UINameGenerator.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "UIComponent/Components/Panel.h"
#include "Utility.h"

UIModule::~UIModule() = default;
UIModule::UIModule() noexcept :
	m_generator{ make_unique<UINameGenerator>() },
	m_component{ nullptr }
{}

bool UIModule::operator==(const UIModule& other) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(m_generator, other.m_generator));
	ReturnIfFalse(CompareUniquePtr(m_component, other.m_component));

	return true;
}

unique_ptr<UIComponent> UIModule::AttachComponent(UIComponent* parent,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	return UIEx(parent).AttachComponent(m_generator.get(), move(child), relativePos);
}

pair<unique_ptr<UIComponent>, UIComponent*> UIModule::DetachComponent(UIComponent* component) noexcept
{
	return UIEx(component).DetachComponent(m_generator.get());
}

void UIModule::Rename(UIComponent* component, const string& name)
{
	UIEx(component).Rename(m_generator.get(), name);
}

bool UIModule::SetupMainComponent(const string& name, const UILayout& layout, IRenderer* renderer)
{
	m_component = CreateComponent<Panel>(layout);
	ReturnIfFalse(m_component->Rename(name));
	ReturnIfFalse(m_component->RenameRegion("UIModuleMainEntry"));
	renderer->AddRenderComponent(m_component.get());

	return true;
}

bool UIModule::Write(const wstring& filename) noexcept
{
	JsonOperation writeJ;
	writeJ.Process("UIComponent", m_component);
	writeJ.Process("UINameGenerator", m_generator);
	ReturnIfFalse(writeJ.Write(!filename.empty() ? filename : m_filename));
	m_filename = filename;

	return true;
}

bool UIModule::Read(const wstring& filename) noexcept
{
	JsonOperation readJ;
	ReturnIfFalse(readJ.Read(!filename.empty() ? filename : m_filename));
	readJ.Process("UIComponent", m_component);
	readJ.Process("UINameGenerator", m_generator);
	m_filename = filename;

	return true;
}