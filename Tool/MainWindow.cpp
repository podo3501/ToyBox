#include "pch.h"
#include "MainWindow.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "../Toy/HelperClass.h"
#include "Popup.h"

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveComponent(m_panel.get());
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_panel{ make_unique<Panel>("Main", GetRectResolution())},
	m_popup{ make_unique<Tool::Popup>( renderer ) }
{
	static int idx{ 0 };
	m_name = "Main Window " + to_string(idx++);
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
	ReturnIfFalse(ReadJsonFile(filename, m_panel));
	m_renderer->AddComponent(m_panel.get(), false);	//메인 창에는 그리지 않는다.
	ReturnIfFalse(m_renderer->LoadComponents());

	const auto& panelSize = m_panel->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(panelSize, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(panelSize);
	m_isOpen = true;

	return true;
}

bool MainWindow::SaveScene(const wstring& filename)
{
	return WriteJsonFile(m_panel, filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
}

const wstring& MainWindow::GetSaveFilename() const noexcept
{
	return m_panel->GetFilename();
}

bool MainWindow::IsFocus() const noexcept
{
	const ImGuiWindow* window = GetImGuiWindow();
	if (window == nullptr || window->Active == false)
		return false;

	ImGuiWindow* focusedWindow = GImGui->NavWindow;
	if (window != focusedWindow)
		return false;

	return true;
}

void MainWindow::ChangeWindowSize(const ImVec2& size)
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->CreateRenderTexture(ImVec2ToXMUINT2(size), m_panel.get(), m_textureID);
	m_size = size;
}

void MainWindow::CheckChangeWindow(const ImGuiWindow* window, MouseTracker* mouseTracker)
{
	static ImVec2 startSize{};
	if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		startSize = window->Size;

	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::RELEASED)
		return;
	
	if(startSize != window->Size && !window->Collapsed)
	{
		ImVec2 newWndSize{ window->Size.x, window->Size.y - GetFrameHeight()};
		ChangeWindowSize(newWndSize);
	}
}

void MainWindow::Update(const DX::StepTimer* timer, MouseTracker* mouseTracker)
{
	if (!IsFocus()) return;
	
	const ImGuiWindow* window = GetImGuiWindow();
	float frameHeight = GetFrameHeight();
	XMUINT2 offset{};
	offset.x = static_cast<uint32_t>(window->Pos.x);
	offset.y = static_cast<uint32_t>(window->Pos.y) + static_cast<uint32_t>(frameHeight);
	mouseTracker->SetOffset(offset);

	//창이 변했을때 RenderTexture를 다시 만들어준다.
	CheckChangeWindow(window, mouseTracker);

	if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		if(m_popup->IsComponent())
		{
			const XMUINT2 size = m_panel->GetSize();
			const ImVec2& pos = m_popup->GetPosition();
			Vector2 curPos{ pos.x / static_cast<float>(size.x), pos.y / static_cast<float>(size.y) };

			m_panel->AddComponent(m_popup->GetComponent(), curPos);
		}
	}

	m_panel->Update({}, mouseTracker);
	m_popup->Excute(mouseTracker);
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

	//ImVec2 windowPos = ImGui::GetWindowPos();

	ImGui::Image(m_textureID, m_size);
	m_popup->Show();
	
	ImGui::End();
}