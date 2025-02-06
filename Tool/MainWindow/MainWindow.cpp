#include "pch.h"
#include "MainWindow.h"
#include "../Utility.h"
#include "ComponentController.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"

int MainWindow::m_mainWindowIndex = 0;

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveRenderComponent(m_panel.get());// �Ƹ� �� ���� �ʿ� ������ ������.
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_name{ "Main Window " + to_string(m_mainWindowIndex++) },
	m_panel{ make_unique<Panel>("Main", 	UILayout(RectangleToXMUINT2(GetRectResolution()), Origin::LeftTop)) },
	m_controller{ make_unique<ComponentController>(renderer, m_panel.get(), m_name) }
{
	//AddRenderComponent�� ���°��� main ȭ�鿡�� �����ִ°� �ƴ϶� TextureRendering�ؼ� �����ִ°ű�
	// ������ Render�� �����Ű�� �ʴ´�.
	m_panel->RenameRegion("MainRegionEntry");
	m_renderer->AddImguiComponent(this);
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	m_panel->SetSize(size);
	ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(size);
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(JsonFile::ReadComponent(filename, m_panel));
	ReturnIfFalse(m_renderer->LoadComponent(m_panel.get()));

	const auto& panelSize = m_panel->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(panelSize, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(panelSize);
	m_isOpen = true;

	m_controller->SetPanel(m_panel.get());

	return true;
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
	return JsonFile::GetJsonFilename(m_panel.get());
}

void MainWindow::ChangeWindowSize(const ImVec2& size)
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->CreateRenderTexture(ImVec2ToXMUINT2(size), m_panel.get(), m_textureID);
	m_size = size;
}

void MainWindow::CheckActiveUpdate() noexcept
{
	if (!IsInputAction(Keyboard::F5, KeyState::Pressed)) return;
	
	m_isActiveUpdate = !m_isActiveUpdate;
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

void MainWindow::Update(const DX::StepTimer* timer)
{
	//if (!IsWindowFocus(m_window)) return;
	if (!m_window) return;
	SetMouseStartOffset(m_window);
	CheckChangeWindow(m_window); //â�� �������� RenderTexture�� �ٽ� ������ش�.

	CheckActiveUpdate();
		
	m_controller->Update();
	m_panel->ProcessUpdate({}, m_isActiveUpdate);
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

void MainWindow::ShowStatusBar() //���� ǥ����(�ӽ�)
{
	if (m_isActiveUpdate)
	{
		ImGui::SetCursorPos({ 0, GetFrameHeight() });
		ImGui::Text("Status update has been activated.");
	}
	else
	{
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
	ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.

	SetupWindowAppearing();

	ImGui::Image(m_textureID, m_size);

	if (m_window && IsWindowFocus(m_window))
	{
		IgnoreMouseClick();
		Tool::MouseCursor::Render();
	}
	
	m_controller->Render();

	ShowStatusBar();

	ImGui::End();
}