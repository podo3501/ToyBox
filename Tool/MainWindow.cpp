#include "pch.h"
#include "MainWindow.h"
#include "../Include/IRenderScene.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
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

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(ReadJsonFile(filename, *m_scene));
	ReturnIfFalse(m_renderer->LoadScenes());
	ReturnIfFalse(m_scene->SetDatas(m_renderer->GetValue()));
	ReturnIfFalse(m_scene->Update(nullptr));	//position 갱신. 안하면 {0, 0}에 1프레임 정도 잠깐 나타난다.

	ReturnIfFalse(m_renderer->CreateRenderTexture({ 800, 600 }, m_scene.get(), m_textureID));
	m_isOpen = true;
	m_filename = filename;

	return true;
}

bool MainWindow::SaveScene() const
{
	return WriteJsonFile(*m_scene, m_filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
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

void MainWindow::Update(const DX::StepTimer* timer, MouseTracker* mouseTracker)
{
	if (!IsFocus()) return;

	const ImGuiWindow* window = GetImGuiWindow();
	float frameHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
	XMUINT2 offset{};
	offset.x = static_cast<uint32_t>(window->Pos.x);
	offset.y = static_cast<uint32_t>(window->Pos.y) + static_cast<uint32_t>(frameHeight);
	mouseTracker->SetOffset(offset);

	m_scene->Update(mouseTracker);
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

	ImVec2 windowPos = ImGui::GetWindowPos();

	ImGui::Image(m_textureID, { 800.f, 600.f });
	ImGui::End();
}