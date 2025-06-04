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

unique_ptr<UIComponent> UIModule::AttachComponent(const string& regionName, const string& name,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	UIComponent* component = FindComponentInRegion(regionName, name);
	if (!component) return child;

	return AttachComponent(component, move(child), relativePos);
}

pair<unique_ptr<UIComponent>, UIComponent*> UIModule::DetachComponent(UIComponent* component) noexcept
{
	return UIEx(component).DetachComponent(m_generator.get());
}

pair<unique_ptr<UIComponent>, UIComponent*> UIModule::DetachComponent(const string& regionName, const string& name) noexcept
{
	UIComponent* component = FindComponentInRegion(regionName, name);
	if (!component) return {};

	return DetachComponent(component);
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

	//renderer->AddRenderComponent(m_component.get());
	m_resBinder = CreateSourceBinder(srcBinderFilename);
	ReturnIfFalse(renderer->LoadTextureBinder(m_resBinder.get()));
	m_renderer = renderer;

	return true;
}

bool UIModule::SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	ReturnIfFalse(Read(filename));

	renderer->AddRenderComponent(m_component.get());
	m_resBinder = CreateSourceBinder(srcBinderFilename);
	ReturnIfFalse(renderer->LoadTextureBinder(m_resBinder.get()));
	m_renderer = renderer;

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

bool UIModule::Update(const DX::StepTimer& timer) noexcept
{
	return m_component->ProcessUpdate(timer);
}

void UIModule::SerializeIO(JsonOperation& operation)
{
	operation.Process("UIComponent", m_component);
	operation.Process("UINameGenerator", m_generator);
}

bool UIModule::Write(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	WriteJsonToFile(*this, curFilename);
	m_filename = curFilename;

	return true;
}

bool UIModule::Read(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	ReadJsonFromFile(curFilename, *this);
	m_filename = curFilename;

	return true;
}

bool UIModule::EnableToolMode(bool enable)
{
	return m_component->EnableToolMode(enable);
}

UIComponent* UIModule::FindComponent(const string& name) const noexcept
{
	return UIEx(m_component).FindComponent(name);
}

UIComponent* UIModule::FindComponentInRegion(const string& regionName, const string& name) const noexcept
{
	UIComponent* regionRoot = UIEx(m_component).GetRegionComponent(regionName);
	if (!regionRoot) return nullptr;

	return UIEx(regionRoot).FindComponent(name);
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