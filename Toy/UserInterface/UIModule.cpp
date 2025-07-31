#include "pch.h"
#include "UIModule.h"
#include "../Include/IRenderer.h"
#include "UIComponent/UIComponent.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "UIComponent/Components/Panel.h"
#include "Utility.h"

UIModule::~UIModule()
{
	Assert(m_panel); //m_component는 꼭 만들어져야 한다.
	m_renderer->RemoveRenderComponent(m_panel.get());
}

UIModule::UIModule() noexcept :
	m_generator{ make_unique<UINameGenerator>() },
	m_panel{ nullptr },
	m_renderer{ nullptr }
{}

bool UIModule::operator==(const UIModule& other) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(m_generator, other.m_generator));
	ReturnIfFalse(CompareUniquePtr(m_panel, other.m_panel));

	return true;
}

//?!? 두 SetupMainComponent 함수 리팩토링 해야 함
bool UIModule::SetupMainComponent(const UILayout& layout, const string& name, 
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_panel = CreateComponent<Panel>(layout);
	ReturnIfFalse(UIEx(m_panel).Rename(name));
	ReturnIfFalse(UIEx(m_panel).RenameRegion("UIModuleMainEntry"));
	Panel* panel = ComponentCast<Panel*>(m_panel.get());
	panel->SetUIModule(this);

	//renderer->AddRenderComponent(m_component.get());
	m_resBinder = CreateSourceBinder(srcBinderFilename);
	ReturnIfFalse(renderer->LoadTextureBinder(m_resBinder.get()));
	m_renderer = renderer;

	return true;
}

bool UIModule::SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	ReturnIfFalse(Read(filename));

	m_resBinder = CreateSourceBinder(srcBinderFilename);
	ReturnIfFalse(renderer->LoadTextureBinder(m_resBinder.get()));
	m_renderer = renderer;

	ReturnIfFalse(BindTextureResources());

	return true;
}

void UIModule::AddRenderer() noexcept
{
	m_renderer->AddRenderComponent(m_panel.get());
}

bool UIModule::BindTextureResources() noexcept
{
	ReturnIfFalse(m_resBinder);
	ReturnIfFalse(m_panel);
	ReturnIfFalse(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

	return true;
}

bool UIModule::Update(const DX::StepTimer& timer) noexcept
{
	return m_panel->ProcessUpdate(timer);
}

void UIModule::Render(ITextureRender* render) const
{
	return m_panel->ProcessRender(render);
}

void UIModule::ReloadDatas() noexcept
{
	Panel* panel = ComponentCast<Panel*>(m_panel.get());
	panel->SetUIModule(this);
}

void UIModule::SerializeIO(JsonOperation& operation)
{
	operation.Process("UIComponent", m_panel);
	operation.Process("UINameGenerator", m_generator);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

bool UIModule::Write(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	JsonOperation::WriteJsonToFile(*this, curFilename);
	m_filename = curFilename;

	return true;
}

bool UIModule::Read(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	JsonOperation::ReadJsonFromFile(curFilename, *this);
	m_filename = curFilename;

	return true;
}

bool UIModule::EnableToolMode(bool enable)
{
	return m_panel->EnableToolMode(enable);
}

UIComponent* UIModule::FindComponent(const string& name) const noexcept
{
	return UIEx(m_panel).FindComponent(name);
}

UIComponent* UIModule::FindComponentInRegion(const string& regionName, const string& name) const noexcept
{
	UIComponent* regionRoot = UIEx(m_panel).GetRegionComponent(regionName);
	if (!regionRoot) return nullptr;

	return UIEx(regionRoot).FindComponent(name);
}

UIComponent* UIModule::GetComponent() const noexcept { return m_panel.get(); }

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

