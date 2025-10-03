#include "pch.h"
#include "UIModule.h"
#include "IRenderer.h"
#include "UIComponent/UIComponent.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "UserInterface/JsonOperation/JsonSerializer.h"
#include "UIComponent/Components/Panel.h"
#include "Shared/Utils/StlExt.h"

UIModule::~UIModule()
{
	auto panel = GetMainPanel();
	Assert(panel);//panel은 꼭 만들어져야 한다.

	m_renderer->RemoveRenderComponent(panel);
}

UIModule::UIModule() noexcept :
	m_generator{ make_unique<UINameGenerator>() },
	m_renderer{ nullptr }
{}

bool UIModule::operator==(const UIModule& other) const noexcept
{
	ReturnIfFalse(Compare(m_generator, other.m_generator));
	ReturnIfFalse(Compare(m_children.front(), other.m_children.front()));

	return true;
}

//?!? 두 SetupMainComponent 함수 리팩토링 해야 함
bool UIModule::SetupMainComponent(const UILayout& layout, const string& name, 
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	unique_ptr<Panel> panel = CreateComponent<Panel>(layout);
	ReturnIfFalse(UIEx(panel).Rename(name));
	ReturnIfFalse(UIEx(panel).RenameRegion("UIModuleMainEntry"));
	panel->SetUIModule(this);
	m_children.push_back(move(panel));

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
	if (auto panel = GetMainPanel())
		m_renderer->AddRenderComponent(panel);
}

bool UIModule::BindTextureResources() noexcept
{
	auto* panel = GetMainPanel();
	ReturnIfFalse(panel);

	ReturnIfFalse(m_resBinder);
	ReturnIfFalse(!m_children.empty());
	ReturnIfFalse(panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

	return true;
}

bool UIModule::Update(const DX::StepTimer& timer) noexcept
{
	auto* panel = GetMainPanel();
	ReturnIfFalse(panel);

	return panel->ProcessUpdate(timer);
}

void UIModule::Render(ITextureRender* render) const
{
	auto* panel = GetMainPanel();
	Assert(panel);

	panel->ProcessRender(render);
}

void UIModule::ReloadDatas() noexcept
{
	auto* panel = GetMainPanel();
	Assert(panel);

	Panel* castPanel = ComponentCast<Panel*>(panel);
	castPanel->SetUIModule(this);
}

void UIModule::SerializeIO(JsonOperation& operation)
{
	operation.Process("Children", m_children);
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
	auto* panel = GetMainPanel();
	ReturnIfFalse(panel);

	return panel->EnableToolMode(enable);
}

UIComponent* UIModule::FindComponent(const string& name) const noexcept
{
	auto* panel = GetMainPanel();
	if(!panel) return nullptr;

	return UIEx(panel).FindComponent(name);
}

UIComponent* UIModule::FindComponentInRegion(const string& regionName, const string& name) const noexcept
{
	auto* panel = GetMainPanel();
	if (!panel) return nullptr;

	UIComponent* regionRoot = UIEx(panel).GetRegionComponent(regionName);
	if (!regionRoot) return nullptr;

	return UIEx(regionRoot).FindComponent(name);
}

UIComponent* UIModule::GetMainPanel() const noexcept 
{ 
	if (m_children.empty())
		return nullptr;

	return m_children.front().get(); 
}

const vector<unique_ptr<UIComponent>>& UIModule::GetChildren() const noexcept { return m_children; }

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

