#include "pch.h"
#include "UIRegistry.h"
#include "UIComponent/UIComponent.h"
#include "UINameGenerator.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "Utility.h"

UIRegistry::~UIRegistry() = default;
UIRegistry::UIRegistry() noexcept :
	m_generator{ make_unique<UINameGenerator>() },
	m_entryComponent{ nullptr }
{
}

unique_ptr<UIComponent> UIRegistry::AttachComponent(UIComponent* parent,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	return UIEx(parent).AttachComponent(m_generator.get(), move(child), relativePos);
}

pair<unique_ptr<UIComponent>, UIComponent*> UIRegistry::DetachComponent(UIComponent* component) noexcept
{
	return UIEx(component).DetachComponent(m_generator.get());
}

void UIRegistry::Rename(UIComponent* component, const string& name)
{
	UIEx(component).Rename(m_generator.get(), name);
}

bool UIRegistry::Save(const wstring& filename) noexcept
{
	JsonOperation writeJ;
	writeJ.Write("UIComponent", m_entryComponent);
	//writeJ.Write("NameGenerator", m_generator.get());
	ReturnIfFalse(writeJ.Write(filename.empty() ? m_filename : filename));
	m_filename = filename;
	return true;
}