#pragma once

struct IRenderer;
class UIComponent;
class InputManager;

class ComponentWindow
{
public:
	ComponentWindow();
	virtual ~ComponentWindow();
	virtual void SetComponent(UIComponent* component);
	
	void Update(InputManager* inputManager);
	void Render(const ImGuiWindow* mainWindow);
	const UIComponent* GetComponent() const noexcept { return m_component; }

protected:
	virtual void UpdateComponent(InputManager* inputManager) {};
	virtual void RenderComponent(UIComponent* component, bool& modify) {};

	bool EditSize(const XMUINT2& size);

private:
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
	ComponentImageGrid1() = delete;
	ComponentImageGrid1(IRenderer* renderer);

	virtual void UpdateComponent(InputManager* inputManager) override;
	virtual void RenderComponent(UIComponent* component, bool& modify) override;

private:
	IRenderer* m_renderer;
	unique_ptr<TextureWindow> m_textureWindow;
};

class ComponentImageGrid3 : public ComponentWindow
{
public:
	virtual void RenderComponent(UIComponent* component, bool& modify) override;

private:
};

class ComponentImageGrid9 : public ComponentWindow
{
public:
	virtual void RenderComponent(UIComponent* component, bool& modify) override;

private:
};