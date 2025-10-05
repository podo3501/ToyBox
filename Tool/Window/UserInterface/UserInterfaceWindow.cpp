#include "pch.h"
#include "UserInterfaceWindow.h"
#include "Window/Utils/Common.h"
#include "ComponentController.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"
#include "Toy/UserInterface/UIModule.h"
#include "Shared/System/StepTimer.h"
#include "Shared/System/Input.h"

int UserInterfaceWindow::m_uiWindowIndex = 0;

UserInterfaceWindow::~UserInterfaceWindow()
{
	m_renderer->RemoveImguiComponent(this);
}

UserInterfaceWindow::UserInterfaceWindow(IRenderer* renderer) :
	InnerWindow{ "UserInterface Window " + to_string(m_uiWindowIndex++) },
	m_renderer{ renderer }
{
	m_renderer->AddImguiComponent(this);
}

ImVec2 UserInterfaceWindow::GetPanelSize() const noexcept
{
	UIComponent* component = GetUIModule()->GetMainPanel();
	return XMUINT2ToImVec2(component->GetSize());
}

bool UserInterfaceWindow::SetupProperty(unique_ptr<UIModule> uiModule)
{
	m_controller = make_unique<ComponentController>(m_renderer, uiModule->GetTexResBinder(),
		uiModule->GetMainPanel(), GetName());

	UIComponent* mainComponent = uiModule->GetMainPanel();
	unique_ptr<UIModuleAsComponent> asComponent = CreateComponent<UIModuleAsComponent>(move(uiModule));
	m_mainRenderTexture = CreateComponent<RenderTexture>(
		UILayout{ mainComponent->GetSize(), Origin::LeftTop }, move(asComponent));
	ReturnIfFalse(m_mainRenderTexture->BindTextureSourceInfo(nullptr, m_renderer->GetTextureController())); //모듈안에 resBinder가 있기 때문에 이것은 nullptr로 한다.

	ToggleToolMode();
	m_isOpen = true;

	return true;
}

bool UserInterfaceWindow::CreateScene(const XMUINT2& size)
{
	unique_ptr<UIModule> module = CreateUIModule(UILayout(size, Origin::LeftTop), 
		"Main", m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
	return SetupProperty(move(module));
}

bool UserInterfaceWindow::CreateScene(const wstring& filename)
{
	unique_ptr<UIModule> module = CreateUIModule(filename,
		m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
	return SetupProperty(move(module));
}

bool UserInterfaceWindow::SaveScene(const wstring& filename)
{
	UIModule* module = GetUIModule();
	return module->Write(filename);
}

wstring UserInterfaceWindow::GetSaveFilename() const noexcept
{
	UIModule* module = GetUIModule();
	return module->GetFilename();
}

void UserInterfaceWindow::ChangeWindowSize(const ImVec2& size)
{
	const XMUINT2& uint2Size = ImVec2ToXMUINT2(size);
	m_mainRenderTexture->ChangeSize(uint2Size);
}

UIModule* UserInterfaceWindow::GetUIModule() const noexcept
{
	UIComponent* component = m_mainRenderTexture->GetRenderedComponent();
	UIModuleAsComponent* asComponent = ComponentCast<UIModuleAsComponent*>(component);
	return asComponent->GetUIModule();
}

void UserInterfaceWindow::ToggleToolMode() noexcept
{
	m_isTool = !m_isTool;
	GetUIModule()->EnableToolMode(m_isTool);
	m_mainRenderTexture->EnableChildMouseEvents(!m_isTool);
	m_controller->SetActive(m_isTool);
}

void UserInterfaceWindow::CheckActiveUpdate() noexcept
{
	if (!IsInputAction(Keyboard::F5, KeyState::Pressed)) return;
	
	ToggleToolMode();
}

void UserInterfaceWindow::CheckChangeWindow(const ImGuiWindow* window)
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

void UserInterfaceWindow::Update(const DX::StepTimer& timer)
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

void UserInterfaceWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	SetupImGuiWindow();
	SetupWindowAppearing();
	HandleMouseEvents();
	RenderContent();

	ImGui::End();
}

void UserInterfaceWindow::SetupWindowAppearing() noexcept
{
	if (!ImGui::IsWindowAppearing()) return;
	
	m_window = GetImGuiWindow();
	m_controller->SetUIWindow(m_window);
}

void UserInterfaceWindow::HandleMouseEvents()
{
	IgnoreMouseClick(m_window);
	if (m_window && IsWindowFocus(m_window))
		Tool::MouseCursor::Render();
}

void UserInterfaceWindow::ShowStatusBar() const//상태 표시줄(임시)
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
		ImGui::Text("Frames per Second: %.4f  --Status update has been activated--", m_fps);
	}	
}

void UserInterfaceWindow::SetupImGuiWindow()
{
	const auto& panelSize = GetPanelSize();
	auto windowSize = ImVec2(panelSize.x, panelSize.y + GetFrameHeight());
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(windowSize);
	ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_None);
	ImGui::PopStyleVar();
}

void UserInterfaceWindow::RenderContent()
{
	ImGui::Image(m_mainRenderTexture->GetGraphicMemoryOffset(), GetPanelSize());
	m_controller->Render();
	ShowStatusBar();
}
