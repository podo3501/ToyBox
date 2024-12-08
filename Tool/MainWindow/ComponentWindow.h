#pragma once

class UIComponent;

class ComponentWindow
{
public:
	ComponentWindow();
	void SetComponent(UIComponent* component);
	void Render();

private:
	bool m_visible{ false };
	UIComponent* m_component;
};