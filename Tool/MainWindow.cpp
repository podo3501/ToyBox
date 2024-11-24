#include "pch.h"
#include "MainWindow.h"
#include "../Include/IRenderScene.h"

MainWindow::~MainWindow()
{
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer }
{
	m_renderer->AddImguiComponent(this);
}

bool MainWindow::CreateScene(const wstring& filename)
{
	filename;
	return false;
}

void MainWindow::Update()
{}

void MainWindow::Render(ImGuiIO* io)
{
}