#include "pch.h"
#include "MainWindow.h"
#include "../Utility.h"
#include "ComponentController.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/UIModule.h"
#include "../Toy/InputManager.h"
#include "../Toy/StepTimer.h"

int MainWindow::m_mainWindowIndex = 0;

MainWindow::~MainWindow()
{
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	InnerWindow{ "Main Window " + to_string(m_mainWindowIndex++) },
	m_renderer{ renderer }
	//m_resBinder{ CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json") }
	//m_panel{ nullptr }
{
	m_renderer->AddImguiComponent(this);
	//m_renderer->LoadTextureBinder(m_resBinder.get());
}

ImVec2 MainWindow::GetPanelSize() const noexcept
{
	//return XMUINT2ToImVec2(m_panel->GetSize());
	UIComponent* component = GetUIModule()->GetComponent();
	return XMUINT2ToImVec2(component->GetSize());
}

//bool MainWindow::SetupProperty(unique_ptr<Panel>&& panel)
//{
//	//AddRenderComponent가 없는것은 main 화면에서 보여주는게 아니라 TextureRendering해서 보여주는거기 때문에 Render에 연결시키지 않는다.
//	m_panel = panel.get();
//	m_panel->RenameRegion("MainRegionEntry");
//	m_controller = make_unique<ComponentController>(m_renderer, m_resBinder.get(), m_panel, GetName());
//
//	const auto& size = m_panel->GetSize();
//	m_renderTex = CreateComponent<RenderTexture>(UILayout{ size, Origin::LeftTop }, move(panel));
//	ReturnIfFalse(m_renderTex->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));
//
//	ToggleToolMode();
//	m_isOpen = true;
//
//	return true;
//}

bool MainWindow::SetupProperty(unique_ptr<UIModule> uiModule)
{
	m_controller = make_unique<ComponentController>(m_renderer, uiModule->GetTexResBinder(),
		uiModule->GetComponent(), GetName());

	UIComponent* mainComponent = uiModule->GetComponent();
	unique_ptr<UIModuleAsComponent> asComponent = CreateComponent<UIModuleAsComponent>(move(uiModule));
	m_mainRenderTexture = CreateComponent<RenderTexture>(
		UILayout{ mainComponent->GetSize(), Origin::LeftTop }, move(asComponent));
	ReturnIfFalse(m_mainRenderTexture->BindTextureSourceInfo(nullptr, m_renderer->GetTextureController())); //모듈안에 resBinder가 있기 때문에 이것은 nullptr로 한다.

	ToggleToolMode();
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	unique_ptr<UIModule> module = CreateUIModule(UILayout(size, Origin::LeftTop), 
		"Main", m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
	return SetupProperty(move(module));

	//auto panel = make_unique<Panel>("Main", UILayout(size, Origin::LeftTop));
	//return SetupProperty(move(panel));
}

bool MainWindow::CreateScene(const wstring& filename)
{
	unique_ptr<UIModule> module = CreateUIModule(filename,
		m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
	return SetupProperty(move(module));

	//unique_ptr<Panel> panel;
	//ReturnIfFalse(JsonFile::ReadComponent(filename, panel));
	//return SetupProperty(move(panel));
}

bool MainWindow::SaveScene(const wstring& filename)
{
	UIModule* module = GetUIModule();
	return module->Write(filename);

	//return JsonFile::WriteComponent(m_panel, filename);
}

wstring MainWindow::GetSaveFilename() const noexcept
{
	UIModule* module = GetUIModule();
	return module->GetFilename();

	//return JsonFile::GetJsonFilename(m_panel);
}

void MainWindow::ChangeWindowSize(const ImVec2& size)
{
	const XMUINT2& uint2Size = ImVec2ToXMUINT2(size);
	m_mainRenderTexture->ChangeSize(uint2Size);
}

UIModule* MainWindow::GetUIModule() const noexcept
{
	UIComponent* component = m_mainRenderTexture->GetRenderedComponent();
	UIModuleAsComponent* asComponent = ComponentCast<UIModuleAsComponent*>(component);
	return asComponent->GetUIModule();
}

void MainWindow::ToggleToolMode() noexcept
{
	m_isTool = !m_isTool;
	//m_panel->EnableToolMode(m_isTool);
	GetUIModule()->EnableToolMode(m_isTool);
	m_mainRenderTexture->EnableChildMouseEvents(!m_isTool);
	m_controller->SetActive(m_isTool);
}

void MainWindow::CheckActiveUpdate() noexcept
{
	if (!IsInputAction(Keyboard::F5, KeyState::Pressed)) return;
	
	ToggleToolMode();
}

void MainWindow::CheckChangeWindow(const ImGuiWindow* window)
{
	static ImVec2 startSize{};
	if (IsInputAction(MouseButton::Left, KeyState::Pressed))
		startSize = window->Size;

	if (!IsInputAction(MouseButton::Left, KeyState::Released))
		return;
	
	if(startSize != window->Size && !window->Collapsed)
	{
		ImVec2 newWndSize{ window->Size.x, window->Size.y - GetFrameHeight()};
		ChangeWindowSize(newWndSize);
	}
}

void MainWindow::Update(const DX::StepTimer& timer)
{
	if (!m_window) return;
	SetMouseStartOffset(m_window);
	CheckChangeWindow(m_window); //창이 변했을때 RenderTexture를 다시 만들어준다.

	CheckActiveUpdate();
		
	m_controller->Update();
	m_mainRenderTexture->ProcessUpdate(timer);

	if (float elapsedTime = static_cast<float>(timer.GetElapsedSeconds()); elapsedTime)
		m_fps = 1.0f / elapsedTime;
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	SetupImGuiWindow();
	SetupWindowAppearing();
	HandleMouseEvents();
	RenderContent();

	ImGui::End();
}

void MainWindow::SetupWindowAppearing() noexcept
{
	if (!ImGui::IsWindowAppearing()) return;
	
	m_window = GetImGuiWindow();
	m_controller->SetMainWindow(m_window);
}

void MainWindow::HandleMouseEvents()
{
	IgnoreMouseClick(m_window);
	if (m_window && IsWindowFocus(m_window))
		Tool::MouseCursor::Render();
}

void MainWindow::ShowStatusBar() const//상태 표시줄(임시)
{
	if (m_isTool)
	{
		ImGui::SetCursorPos({ 0, GetFrameHeight() });
		ImGui::Text("Frames per Second: %.4f", m_fps);
		ImGui::SetCursorPos(ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFontSize()));
		ImGui::Text("Right Mouse Button: Floating Menu     Shift + Left Mouse Button: Attach     D: Detach     B: Clone     Del: Delete     F5: Update State     Ctrl + Z, Y: Undo/Redo");
	}
	else
	{
		ImGui::SetCursorPos({ 0, GetFrameHeight() });
		ImGui::Text("Status update has been activated.");
	}	
}

void MainWindow::SetupImGuiWindow()
{
	const auto& panelSize = GetPanelSize();
	auto windowSize = ImVec2(panelSize.x, panelSize.y + GetFrameHeight());
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_None);
	ImGui::PopStyleVar();
}

void MainWindow::RenderContent()
{
	ImGui::Image(m_mainRenderTexture->GetGraphicMemoryOffset(), GetPanelSize());
	m_controller->Render();
	ShowStatusBar();
}
