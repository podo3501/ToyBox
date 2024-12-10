#pragma once

class UILayout;
class UIComponent;

class ComponentWindow
{
public:
	ComponentWindow();
	virtual ~ComponentWindow();
	virtual void SetComponent(UIComponent* component);
	virtual void Render();
	
	const UIComponent* GetComponent() const noexcept { return m_component; }

protected:
	void RenderRectangleInput(UILayout& layout);
	bool RenderIntegerInput(const char* label, long& value, int min = 0, int max = 10000);

private:
	bool m_visible{ false };
	UIComponent* m_component;
};

class ComponentPanel : public ComponentWindow
{
public:
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};

class ComponentBGImage : public ComponentWindow
{
public:
	/*virtual void SetComponent(UIComponent* component);
	virtual void Render();*/

private:
};