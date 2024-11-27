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
	m_name{ "MainWindow" },
	m_scene{ make_unique<Scene>(GetRectResolution()) }
{
	m_renderer->AddImguiComponent(this);
	m_renderer->AddLoadScene(m_scene.get());
	//m_renderer->AddRenderScene(m_scene.get());	//메인 창에는 그리지 않는다.
}

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(ReadScene(filename, *m_scene));
	ReturnIfFalse(m_renderer->LoadScenes());

	ReturnIfFalse(m_renderer->CreateRenderTexture({ 800, 600 }, m_scene.get(), m_textureID));
	m_visible = true;

	return true;
}

void MainWindow::Update(const DX::StepTimer* timer, CustomButtonStateTracker* mouseTracker)
{
	ImGuiWindow* window = ImGui::FindWindowByName(m_name.c_str());
	if (window == nullptr || window->Active == false)
		return;

	ImGuiWindow* focusedWindow = GImGui->NavWindow;
	if (window != focusedWindow)
		return;

	float frameHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
	XMUINT2 offset{};
	offset.x = static_cast<uint32_t>(window->Pos.x);
	offset.y = static_cast<uint32_t>(window->Pos.y) + static_cast<uint32_t>(frameHeight);
	mouseTracker->SetOffset(offset);

	m_scene->Update(mouseTracker);
}

bool isPinned = false;

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_visible)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(m_name.c_str(), &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

	ImVec2 windowPos = ImGui::GetWindowPos();

	ImGui::Image(m_textureID, { 800.f, 600.f });
	ImGui::End();
}