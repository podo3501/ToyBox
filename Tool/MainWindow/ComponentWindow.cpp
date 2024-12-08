#include "pch.h"
#include "ComponentWindow.h"
#include "../Toy/UserInterface/UIComponent.h"

ComponentWindow::ComponentWindow() :
	m_component{ nullptr }
{}

void ComponentWindow::SetComponent(UIComponent* component)
{
	m_component = component;
	m_visible = (m_component != nullptr);
}

void ComponentWindow::Render()
{
	if (!m_visible) return;

	ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::End();
}
