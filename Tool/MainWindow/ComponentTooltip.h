#pragma once

class UIComponent;
class InputManager;

class ComponentTooltip
{
public:
	ComponentTooltip(UIComponent* panel);
	~ComponentTooltip();

	void SelectComponent(UIComponent* component) noexcept;
	void CheckSelectedComponent(InputManager* inputManager) noexcept;
	void ShowTooltip(const ImGuiWindow* window);

	UIComponent* GetComponent() const noexcept { return m_selectCom; }

private:
	void RepeatedSelection(const std::vector<UIComponent*>& componentList);
	void ShowTooltipComponent(bool isSelected, 
		const ImVec2& tooltipPos, const ImVec2& tooltipSize, const string& id, const string& context);

	UIComponent* m_panel;
	UIComponent* m_selectCom;
};