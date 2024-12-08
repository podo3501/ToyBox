#pragma once

class ComponentWindow;
class UIComponent;
class InputManager;

class ComponentTooltip
{
public:
	ComponentTooltip(UIComponent* panel, ComponentWindow* comWindow);
	~ComponentTooltip();

	void SelectComponent(UIComponent* component) noexcept;
	void CheckSelectedComponent(InputManager* inputManager) noexcept;
	void ShowTooltip(const ImGuiWindow* window);

private:
	void RepeatedSelection(const std::vector<UIComponent*>& componentList);
	void ShowTooltipComponent(bool isSelected, 
		const ImVec2& tooltipPos, const ImVec2& tooltipSize, const string& id, const string& context);

	UIComponent* m_panel;
	ComponentWindow* m_comWindow;
	UIComponent* m_selectCom;
};