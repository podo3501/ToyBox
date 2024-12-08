#include "pch.h"
#include "ComponentTooltip.h"
#include "ComponentWindow.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/Utility.h"
#include "../Toy/HelperClass.h"
#include "../Utility.h"

ComponentTooltip::~ComponentTooltip() = default;
ComponentTooltip::ComponentTooltip(UIComponent* panel, ComponentWindow* comWindow) :
	m_panel{ panel },
	m_comWindow{ comWindow },
	m_selectCom{ nullptr }
{}

void ComponentTooltip::SelectComponent(UIComponent* component) noexcept
{
	if (m_selectCom != nullptr)
		m_selectCom->SetSelected(false);

	m_selectCom = component;

	if (m_selectCom)
		m_selectCom->SetSelected(true);

	m_comWindow->SetComponent(m_selectCom);
}

void ComponentTooltip::CheckSelectedComponent(InputManager* inputManager) noexcept
{
	auto pressedKey = inputManager->GetKeyboard()->pressed;
	if (pressedKey.Escape)
	{
		SelectComponent(nullptr);
		return;
	}

	auto mouseTracker = inputManager->GetMouse();
	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::PRESSED) return;	//왼쪽버튼 눌렀을때 

	static vector<UIComponent*> preComponentList{ nullptr };
	vector<UIComponent*> componentList;
	const XMINT2& pos = mouseTracker->GetOffsetPosition();
	m_panel->GetComponents(pos, componentList);
	if (componentList.empty()) return;

	if (preComponentList == componentList)
		RepeatedSelection(componentList);
	else //마우스가 다른 컴포넌트를 선택해서 리스트가 바뀌었다면 제일 첫번째 원소를 찍어준다.
	{
		SelectComponent(componentList.back());
		preComponentList = move(componentList);
	}
}

void ComponentTooltip::RepeatedSelection(const std::vector<UIComponent*>& componentList)
{
	auto findIdx = FindIndex(componentList, m_selectCom);
	if (!findIdx.has_value())
	{
		SelectComponent(componentList.back());
		return;
	}

	int idx = findIdx.value() - 1;
	if (idx < 0) idx = static_cast<int>(componentList.size() - 1);
	SelectComponent(componentList[idx]);
}

void ComponentTooltip::ShowTooltip(const ImGuiWindow* window)
{
	const ImVec2& windowMousePos = GetMousePosition(window);

	std::vector<UIComponent*> componentList;
	m_panel->GetComponents(ImVec2ToXMINT2(windowMousePos), componentList);
	if (componentList.empty()) return;

	const ImVec2& padding = ImGui::GetStyle().WindowPadding;
	const ImVec2& mousePos = ImGui::GetMousePos();

	float tooltipOffsetY = 20.f; // 초기 Y 오프셋
	const float tooltipGap = 5.f; // 툴팁 간 간격

	for (int idx{ 0 }; UIComponent* curComponent : views::reverse(componentList))
	{
		// 툴팁 위치와 크기 계산
		const std::string& tooltipContext = curComponent->GetType();
		const ImVec2& tooltipPos = ImVec2(mousePos.x + 20, mousePos.y + tooltipOffsetY);
		const ImVec2& textSize = ImGui::CalcTextSize(tooltipContext.c_str());
		const ImVec2& tooltipSize = ImVec2(textSize.x + padding.x * 2, textSize.y + padding.y * 2);

		bool isSelected = (curComponent == m_selectCom);
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