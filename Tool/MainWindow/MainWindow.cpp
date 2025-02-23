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

bool MainWindow::SetupProperty(unique_ptr<Panel>&& panel)
{
	auto panelPtr = panel.get();
	const auto& size = panelPtr->GetSize();
	m_renderTex = CreateRenderTexture({ size, Origin::LeftTop }, move(panel));
	m_controller = make_unique<ComponentController>(m_renderer, panelPtr, m_name);
	//AddRenderComponent가 없는것은 main 화면에서 보여주는게 아니라 TextureRendering해서 보여주는거기 때문에 Render에 연결시키지 않는다.
	panelPtr->RenameRegion("MainRegionEntry");

	ReturnIfFalse(m_renderer->LoadComponent(m_renderTex.get()));
	m_renderTex->EnableChildMouseEvents(m_isActiveUpdate);
	m_size = XMUINT2ToImVec2(size);
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	auto panel = make_unique<Panel>("Main", UILayout(size, Origin::LeftTop));
	m_panel = panel.get();

	return SetupProperty(move(panel));
}

bool MainWindow::CreateScene(const wstring& filename)
{
	unique_ptr<Panel> panel;
	ReturnIfFalse(JsonFile::ReadComponent(filename, panel));
	ReturnIfFalse(m_renderer->LoadComponent(panel.get()));
	m_panel = panel.get();

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
	m_renderTex->ModifyTexture(ImVec2ToXMUINT2(size));
	m_size = size;
}

void MainWindow::CheckActiveUpdate() noexcept
{
	if (!IsInputAction(Keyboard::F5, KeyState::Pressed)) return;
	
	m_isActiveUpdate = !m_isActiveUpdate;
	m_renderTex->EnableChildMouseEvents(m_isActiveUpdate);
	m_controller->SetActive(!m_isActiveUpdate);
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
	//if (!IsWindowFocus(m_window)) return;
	if (!m_window) return;
	SetMouseStartOffset(m_window);
	CheckChangeWindow(m_window); //창이 변했을때 RenderTexture를 다시 만들어준다.

	CheckActiveUpdate();
		
	m_controller->Update();
	m_renderTex->ProcessUpdate(timer);
	//m_panel->ProcessUpdate(timer);

	if (float elapsedTime = static_cast<float>(timer.GetElapsedSeconds()); elapsedTime)
		m_fps = 1.0f / elapsedTime;
}

void MainWindow::IgnoreMouseClick() 
{
	const ImVec2& rectMin = GetWindowStartPosition(m_window);
	const ImVec2& rectMax = rectMin + m_size;
	if (!ImGui::IsMouseHoveringRect(rectMin, rectMax)) return;
	
	ImGui::GetIO().MouseDown[0] = false;
}

void MainWindow::SetupWindowAppearing() noexcept
{
	if (!ImGui::IsWindowAppearing()) return;
	
	m_window = const_cast<ImGuiWindow*>(GetImGuiWindow());
	m_controller->SetMainWindow(m_window);
}

void MainWindow::ShowStatusBar() //상태 표시줄(임시)
{
	if (m_isActiveUpdate)
	{
		ImGui::SetCursorPos({ 0, GetFrameHeight() });
		ImGui::Text("Status update has been activated.");
	}
	else
	{
		ImGui::SetCursorPos({ 0, GetFrameHeight() });
		ImGui::Text("Frames per Second: %.4f", m_fps);
		ImGui::SetCursorPos(ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFontSize()));
		ImGui::Text("Right Mouse Button: Floating Menu     Shift + Left Mouse Button: Attach     D: Detach     B: Clone     Del: Delete     F5: Update State     Ctrl + Z, Y: Undo/Redo");
	}
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize({ m_size.x, m_size.y + GetFrameHeight() });
	ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
	//ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

	SetupWindowAppearing();

	ImGui::Image(m_renderTex->GetGraphicMemoryOffset(), m_size);

	if (m_window && IsWindowFocus(m_window))
	{
		IgnoreMouseClick();
		Tool::MouseCursor::Render();
	}
	
	m_controller->Render();

	ShowStatusBar();

	ImGui::End();
}