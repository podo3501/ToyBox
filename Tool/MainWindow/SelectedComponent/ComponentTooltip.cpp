#include "pch.h"
#include "ComponentTooltip.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/Utility.h"
#include "../Toy/HelperClass.h"
#include "../../Utility.h"

ComponentTooltip::~ComponentTooltip() = default;
ComponentTooltip::ComponentTooltip(UIComponent* panel) :
	m_panel{ panel },
	m_component{ nullptr }
{}

void ComponentTooltip::SetComponent(UIComponent* component) noexcept
{
	m_component = component;
}

void ComponentTooltip::Render(const ImGuiWindow* window)
{
	const ImVec2& windowMousePos = GetMousePosition(window);

	std::vector<UIComponent*> componentList;
	m_panel->GetComponents(ImVec2ToXMINT2(windowMousePos), componentList);
	if (componentList.empty()) return;

	const ImVec2& padding = ImGui::GetStyle().WindowPadding;
	const ImVec2& mousePos = ImGui::GetMousePos();

	constexpr float tooltipOffsetX = 20.f;
	constexpr float tooltipGap = 5.f; // ���� �� ����
	float tooltipOffsetY = 20.f; // �ʱ� Y ������

	for (int idx{ 0 }; UIComponent * curComponent : views::reverse(componentList))
	{
		// ���� ��ġ�� ũ�� ���
		const std::string& tooltipContext = curComponent->GetType();
		const ImVec2& tooltipPos = ImVec2(mousePos.x + tooltipOffsetX, mousePos.y + tooltipOffsetY);
		const ImVec2& textSize = ImGui::CalcTextSize(tooltipContext.c_str());
		const ImVec2& tooltipSize = ImVec2(textSize.x + padding.x * 2, textSize.y + padding.y * 2);

		bool isSelected = (curComponent == m_component);
		const string& windowId = "tooltip_" + std::to_string(idx++);
		ShowTooltipComponent(isSelected, tooltipPos, tooltipSize, windowId, tooltipContext);
		tooltipOffsetY += ImGui::CalcTextSize(curComponent->GetType().c_str()).y + padding.y * 2 + tooltipGap;
	}
}

void ComponentTooltip::ShowTooltipComponent(bool isSelected, 
	const ImVec2& tooltipPos, const ImVec2& tooltipSize, const string& id, const string& context)
{
	ImGui::SetNextWindowPos(tooltipPos);
	ImGui::SetNextWindowSize(tooltipSize);

	if (isSelected) ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::Begin(id.c_str(), nullptr, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	if (isSelected) ImGui::PopStyleColor();

	ImGui::Text(context.c_str());
	ImGui::End();
}