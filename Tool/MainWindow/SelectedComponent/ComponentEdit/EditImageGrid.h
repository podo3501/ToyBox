#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class ImageGrid1;
class EditImageGrid1 : public EditWindow
{
public:
	~EditImageGrid1();
	EditImageGrid1() = delete;
	EditImageGrid1(ImageGrid1* imgGrid1, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	ImageGrid1* m_imageGrid1;
};

class ImageGrid3;
class EditImageGrid3 : public EditWindow
{
public:
	~EditImageGrid3();
	EditImageGrid3() = delete;
	EditImageGrid3(ImageGrid3* imgGrid3, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	ImageGrid3* m_imageGrid3;
};

class ImageGrid9;
class EditImageGrid9 : public EditWindow
{
public:
	~EditImageGrid9();
	EditImageGrid9() = delete;
	EditImageGrid9(ImageGrid9* imgGrid9, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	ImageGrid9* m_imageGrid9;
};
