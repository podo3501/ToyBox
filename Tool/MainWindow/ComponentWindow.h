#pragma once

struct IRenderer;
class UIComponent;

class ComponentWindow
{
public:
	ComponentWindow();
	virtual ~ComponentWindow();
	virtual void SetComponent(UIComponent* component);
	virtual void UpdateComponent() {};
	virtual void RenderComponent(UIComponent* component) {};
	
	void Update();
	void Render();
	const UIComponent* GetComponent() const noexcept { return m_component; }
	void SetRenderer(IRenderer* renderer) noexcept { m_renderer = renderer; }

protected:
	void EditSize(const XMUINT2& size);
	IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
	IRenderer* m_renderer;
	UIComponent* m_component;
	bool m_visible{ false };
};

class ComponentPanel : public ComponentWindow
{
public:
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};

class TextureWindow;

class ComponentImageGrid1 : public ComponentWindow
{
public:
	~ComponentImageGrid1();
	ComponentImageGrid1();

	virtual void UpdateComponent() override;
	virtual void RenderComponent(UIComponent* component) override;

private:
	unique_ptr<TextureWindow> m_textureWindow;
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