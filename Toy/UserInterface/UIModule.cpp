#include "pch.h"
#include "UIModule.h"
#include "../Include/IRenderer.h"
#include "UIComponent/UIComponent.h"
#include "UINameGenerator.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "UIComponent/Components/Panel.h"
#include "Utility.h"

UIModule::~UIModule()
{
	Assert(m_component); //m_component는 꼭 만들어져야 한다.
	m_renderer->RemoveRenderComponent(m_component.get());
}

UIModule::UIModule() noexcept :
	m_generator{ make_unique<UINameGenerator>() },
	m_component{ nullptr },
	m_renderer{ nullptr }
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
//?!? 두 SetupMainComponent 함수 리팩토링 해야 함
bool UIModule::SetupMainComponent(const UILayout& layout, const string& name, 
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_component = CreateComponent<Panel>(layout);
	ReturnIfFalse(m_component->Rename(name));
	ReturnIfFalse(m_component->RenameRegion("UIModuleMainEntry"));
	renderer->AddRenderComponent(m_component.get());

	m_resBinder = CreateSourceBinder(srcBinderFilename);
	ReturnIfFalse(renderer->LoadTextureBinder(m_resBinder.get()));
	m_renderer = renderer;

	return true;
}

bool UIModule::SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	ReturnIfFalse(Read(filename));

	m_resBinder = CreateSourceBinder(srcBinderFilename);
	m_renderer = renderer;
	ReturnIfFalse(m_renderer->LoadTextureBinder(m_resBinder.get()));
	ReturnIfFalse(BindTextureResources());

	return true;
}

bool UIModule::BindTextureResources() noexcept
{
	ReturnIfFalse(m_resBinder);
	ReturnIfFalse(m_component);
	ReturnIfFalse(m_component->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

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

unique_ptr<UIModule> CreateUIModule(const UILayout& layout, const string& mainUIName, 
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	unique_ptr<UIModule> module = make_unique<UIModule>();
	if(!module->SetupMainComponent(layout, mainUIName, renderer, srcBinderFilename)) return nullptr;

	return module;
}

unique_ptr<UIModule> CreateUIModule(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	unique_ptr<UIModule> module = make_unique<UIModule>();
	if (!module->SetupMainComponent(filename, renderer, srcBinderFilename)) return nullptr;

	return module;
}