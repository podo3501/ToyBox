#include "pch.h"
#include "MainWindow.h"
#include "../Include/IRenderScene.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "../Toy/HelperClass.h"

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveLoadScene(m_scene.get());
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_scene{ make_unique<Scene>(GetRectResolution()) }
{
	static int idx{ 0 };
	m_name = "Main Window " + to_string(idx++);
	m_renderer->AddImguiComponent(this);
	m_renderer->AddLoadScene(m_scene.get());
	//m_renderer->AddRenderScene(m_scene.get());	//메인 창에는 그리지 않는다.
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	m_scene->SetSize(size);
	ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_scene.get(), m_textureID));
	m_size = XMUINT2ToImVec2(size);
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(LoadScene(filename, m_scene.get(), m_renderer));
		
	const auto& sceneSize = m_scene->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(sceneSize, m_scene.get(), m_textureID));
	m_size = XMUINT2ToImVec2(sceneSize);
	m_isOpen = true;

	return true;
}

bool MainWindow::SaveScene(const wstring& filename)
{
	return m_scene->SaveFile(filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
}

const wstring& MainWindow::GetSaveFilename() const noexcept
{
	return m_scene->GetFilename();
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
	m_renderer->CreateRenderTexture(ImVec2ToXMUINT2(size), m_scene.get(), m_textureID);
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

	m_scene->Update(mouseTracker);
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

			// 마우스 오른쪽 버튼 클릭 시 팝업 메뉴 띄우기
	if (ImGui::BeginPopupContextWindow("PopupMenu"))
	{
		if (ImGui::MenuItem("Option 1")) {
			// Option 1 선택 시 동작
		}
		if (ImGui::MenuItem("Option 2")) {
			// Option 2 선택 시 동작
		}
		if (ImGui::MenuItem("Close")) {
			// Close 선택 시 동작
		}
		ImGui::EndPopup();
	}

	ImGui::Image(m_textureID, m_size);
	ImGui::End();
}