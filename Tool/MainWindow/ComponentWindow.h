#pragma once

class UILayout;
class UIComponent;
class BGImage;

class ComponentWindow
{
public:
	ComponentWindow();
	virtual ~ComponentWindow();
	virtual void SetComponent(UIComponent* component);
	virtual void RenderComponent(UIComponent* component) {};
	
	void Render();
	const UIComponent* GetComponent() const noexcept { return m_component; }

protected:
	void RenderRectangleInput(const UILayout& layout);
	bool RenderIntegerInput(const char* label, uint32_t& value, int min = 0, int max = 10000);

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

class ComponentImageGrid1 : public ComponentWindow
{
public:
	virtual void RenderComponent(UIComponent* component) override;

private:
};

class ComponentImageGrid3 : public ComponentWindow
{
public:
	virtual void RenderComponent(UIComponent* component) override;

private:
};

class ComponentImageGrid9 : public ComponentWindow
{
public:
	virtual void RenderComponent(UIComponent* component) override;

private:
};