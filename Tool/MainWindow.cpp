#include "pch.h"
#include "MainWindow.h"
#include "../Include/IRenderScene.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"

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
	m_renderer->AddImguiComponent(this);
	m_renderer->AddLoadScene(m_scene.get());
	//m_renderer->AddRenderScene(m_scene.get());	//���� â���� �׸��� �ʴ´�.
}

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(ReadScene(filename, *m_scene));
	ReturnIfFalse(m_renderer->LoadScenes());

	ReturnIfFalse(m_renderer->CreateRenderTexture({ 800, 600 }, m_scene.get(), m_textureID));
	m_visible = true;

	return true;
}

void MainWindow::Update(const DX::StepTimer* timer, const Mouse::ButtonStateTracker* mouseTracker)
{
	m_scene->Update(mouseTracker);
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_visible)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("App Window", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.

	ImGui::Image(m_textureID, { 800.f, 600.f });
	ImGui::End();

	//if (!m_visible)	//â�� �����ٸ�
	//{
	//	auto shared_this = shared_from_this();
	//	shared_this.reset();
	//}
}