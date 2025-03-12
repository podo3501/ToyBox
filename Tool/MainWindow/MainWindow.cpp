#include "pch.h"
#include "MainWindow.h"
#include "../Utility.h"
#include "ComponentController.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"
#include "../Toy/StepTimer.h"

int MainWindow::m_mainWindowIndex = 0;

MainWindow::~MainWindow()
{
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_name{ "Main Window " + to_string(m_mainWindowIndex++) },
	m_panel{ nullptr }
{
	m_renderer->AddImguiComponent(this);
}

ImVec2 MainWindow::GetPanelSize() const noexcept
{
	return XMUINT2ToImVec2(m_panel->GetSize());
}

bool MainWindow::SetupProperty(unique_ptr<Panel>&& panel)
{
	//AddRenderComponent가 없는것은 main 화면에서 보여주는게 아니라 TextureRendering해서 보여주는거기 때문에 Render에 연결시키지 않는다.
	m_panel = panel.get();
	m_panel->RenameRegion("MainRegionEntry");
	m_controller = make_unique<ComponentController>(m_renderer, m_panel, m_name);

	const auto& size = m_panel->GetSize();
	m_renderTex = CreateRenderTexture({ size, Origin::LeftTop }, move(panel));
	ReturnIfFalse(m_renderer->LoadComponent(m_renderTex.get()));

	ToggleToolMode();
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	auto panel = make_unique<Panel>("Main", UILayout(size, Origin::LeftTop));
	return SetupProperty(move(panel));
}

bool MainWindow::CreateScene(const wstring& filename)
{
	unique_ptr<Panel> panel;
	ReturnIfFalse(JsonFile::ReadComponent(filename, panel));
	ReturnIfFalse(m_renderer->LoadComponent(panel.get()));

	return SetupProperty(move(panel));
}

bool MainWindow::SaveScene(const wstring& filename)
{
	return JsonFile::WriteComponent(m_panel, filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
}

wstring MainWindow::GetSaveFilename() const noexcept
{
	return JsonFile::GetJsonFilename(m_panel);
}

void MainWindow::ChangeWindowSize(const ImVec2& size)
{
	const XMUINT2& uint2Size = ImVec2ToXMUINT2(size);
	m_renderTex->ModifyTexture(uint2Size);
	m_panel->ChangeSize(uint2Size);
}

void MainWindow::ToggleToolMode() noexcept
{
	m_isTool = !m_isTool;
	m_renderTex->EnableChildMouseEvents(!m_isTool);
	SetRenderFilterFlag(m_isTool ? StateFlag::Render | StateFlag::RenderEditable : StateFlag::Render);
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
	m_renderTex->ProcessUpdate(timer);

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
	
	m_window = const_cast<ImGuiWindow*>(GetImGuiWindow());
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
	ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
	ImGui::PopStyleVar();
}

void MainWindow::RenderContent()
{
	ImGui::Image(m_renderTex->GetGraphicMemoryOffset(), GetPanelSize());
	m_controller->Render();
	ShowStatusBar();
}
