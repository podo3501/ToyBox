#pragma once
#include "../EditWindow.h"

class TextureWindow;

class EditImageGrid : public EditWindow
{
public:
	~EditImageGrid();
	EditImageGrid() = delete;

	virtual void UpdateComponent(InputManager* inputManager) override;
	virtual void RenderComponent(UIComponent* component, bool& posModify) override;

protected:
	EditImageGrid(IRenderer* renderer) noexcept;
	virtual void RenderComponentEdit(UIComponent* component, bool& posModify) {};

	void RenderExtractTextureButton(const wstring& filename, UIComponent* component);
	inline IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:	//protected로 선언했기 때문에 변수를 늘려야  할때에는 신중을 기한다.
	IRenderer* m_renderer;
	unique_ptr<TextureWindow> m_textureWindow;
};

class EditImageGrid1 : public EditImageGrid
{
public:
	~EditImageGrid1();
	EditImageGrid1() = delete;
	EditImageGrid1(IRenderer* renderer);

protected:
	virtual void RenderComponentEdit(UIComponent* component, bool& posModify) override;
};

class EditImageGrid3 : public EditImageGrid
{
public:
	~EditImageGrid3();
	EditImageGrid3() = delete;
	EditImageGrid3(IRenderer* renderer);

protected:
	virtual void RenderComponentEdit(UIComponent* component, bool& modify) override;
};

class EditImageGrid9 : public EditWindow
{
public:
	virtual void RenderComponent(UIComponent* component, bool& modify) override;

private:
};
