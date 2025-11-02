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
	m_generator{ make_unique<UINameGenerator>() }
{}

UIModule::UIModule(const UIModule& other)
{
	m_generator = other.m_generator->Clone();
	m_resBinder = make_unique<TextureResourceBinder>(*other.m_resBinder);
	m_renderer = other.m_renderer;
	m_filename = other.m_filename;
	
	ranges::transform(other.m_children, back_inserter(m_children), [this, &other](const auto& child) {
		return move(child->Clone());
		});
}

bool UIModule::operator==(const UIModule& other) const noexcept
{
	ReturnIfFalse(Compare(m_generator, other.m_generator));
	ReturnIfFalse(Compare(m_children.front(), other.m_children.front()));
	//?!? 다른 멤버변수들도 해 주자.

	return true;
}

unique_ptr<UIModule> UIModule::Clone() const
{
	return unique_ptr<UIModule>(new UIModule(*this));
}

//?!? 두 SetupMainComponent 함수 리팩토링 해야 함
bool UIModule::SetupMainComponent(const UILayout& layout, const string& name, 
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_resBinder = CreateTextureResourceBinder(srcBinderFilename, renderer);
	ReturnIfFalse(m_resBinder);
	m_renderer = renderer;

	unique_ptr<Panel> panel = CreateComponent<Panel>(layout);
	ReturnIfFalse(UIEx(panel).Rename(name));
	ReturnIfFalse(UIEx(panel).RenameRegion("UIModuleMainEntry"));
	panel->SetUIModule(this);
	m_children.push_back(move(panel));

	return true;
}

bool UIModule::SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_resBinder = CreateTextureResourceBinder(srcBinderFilename, renderer);
	m_renderer = renderer;

	ReturnIfFalse(Read(filename));
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
	ReturnIfFalse(UpdateMainPanel(timer));
	UpdateMouseState();

	return true;
}

bool UIModule::UpdateMainPanel(const DX::StepTimer& timer) noexcept
{
	auto* panel = GetMainPanel();
	return panel && panel->ProcessUpdate(timer);
}

void UIModule::UpdateMouseState() noexcept
{
	//hover는 공통으로 호출
	auto input = InputLocator::GetService();
	auto mouseState = input->GetMouseState();
	auto components = UIEx(GetMainPanel()).FindRenderComponents(mouseState.pos);

	UpdateHoverState(components);
	ProcessCaptureComponent(mouseState); //캡쳐된 컴포넌트를 마우스 입력에 따라 처리
	CaptureComponent(mouseState.leftButton);	//클릭하면 캡쳐하고 press호출
}

void UIModule::UpdateHoverState(vector<UIComponent*> components) noexcept
{
	for (auto& component : components)
		component->OnHover(); //일단 다 호출하고나서

	for (auto& prevComp : m_hoveredComponents)
	{
		if (ranges::find(components, prevComp) == components.end())
			prevComp->OnNormal(); //영역이 아닌 애들은 OnNormal 호출
	}

	m_hoveredComponents = components;
}

void UIModule::ProcessCaptureComponent(const MouseState& mouseState) noexcept
{
	if (!m_capture) return;

	bool inside = Contains(m_capture->GetArea(), mouseState.pos);
	if (!mouseState.leftButton) //3. 마우스를 떼면 release호출하고 캡쳐해제
	{
		m_capture->OnRelease(inside);
		m_capture = nullptr;
	}
	else
		m_capture->OnHold(inside); //2. 캡쳐한걸 hold로 호출한다.
}

void UIModule::CaptureComponent(bool leftButton) noexcept
{
	if (m_capture) return;
	if (!leftButton) return;

	for (auto& component : m_hoveredComponents)
	{
		if (component->OnPress()) //윗 컴포넌트부터 처리하다가 처리가 됐다면 그 밑에는 안한다.
		{
			m_capture = component;
			break;
		}
	}
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

