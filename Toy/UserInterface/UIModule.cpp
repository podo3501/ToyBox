#include "pch.h"
#include "UIModule.h"
#include "IRenderer.h"
#include "Locator/InputLocator.h"
#include "UIComponent/UIComponent.h"
#include "UIComponent/Components/Panel.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/SerializerIO/ClassSerializeIO.h"
#include "UINameGenerator/UINameGenerator.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/GeometryExt.h"
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

bool UIModule::UpdateMouseState() noexcept
{
	//hover는 공통으로 호출
	auto input = InputLocator::GetService();
	auto mouseState = input->GetMouseState();
	auto components = UIEx(GetMainPanel()).FindRenderComponents(mouseState.pos);
	for (auto& component : components)
		component->OnHover();

	if (m_capture)
	{
		bool inside = Contains(m_capture->GetArea(), mouseState.pos);
		if (!mouseState.leftButton) //3. 마우스를 떼면 release호출하고 캡쳐해제
		{
			m_capture->OnRelease(inside);
			m_capture = nullptr;
		}
		else
			m_capture->OnHold(inside); //2. 캡쳐한걸 hold로 호출한다.
		return true;
	}

	//1. 클릭하면 캡쳐하고 press호출
	for (auto& component : components)
	{
		if (component->OnPress())
		{
			m_capture = component;
			break;
		}
	}

	return true;
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

void UIModule::ProcessIO(SerializerIO& serializer)
{
	serializer.Process("Children", m_children);
	serializer.Process("UINameGenerator", m_generator);

	if (serializer.IsWrite()) return;
	ReloadDatas();
}

bool UIModule::Write(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	SerializerIO::WriteJsonToFile(*this, curFilename);
	m_filename = curFilename;

	return true;
}

bool UIModule::Read(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	SerializerIO::ReadJsonFromFile(curFilename, *this);
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

	UIComponent* regionRoot = UIEx(panel).FindRegionComponent(regionName);
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

