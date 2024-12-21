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
	void Render(const ImGuiWindow* window);

	UIComponent* GetComponent() const noexcept { return m_selectCom; }
	void SetPanel(UIComponent* panel) noexcept { m_panel = panel; }

private:
	void RepeatedSelection(const std::vector<UIComponent*>& componentList);
	void ShowTooltip(const ImGuiWindow* window);
	void ShowTooltipComponent(bool isSelected, 
		const ImVec2& tooltipPos, const ImVec2& tooltipSize, const string& id, const string& context);
	void ShowSelectComponent();

	UIComponent* m_panel;
	UIComponent* m_selectCom;
};