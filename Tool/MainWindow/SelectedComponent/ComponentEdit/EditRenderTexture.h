#pragma once
#include "EditWindow.h"

class TextureSourceBinder;
class RenderTexture;
class EditRenderTexture : public EditWindow
{
public:
	~EditRenderTexture();
	EditRenderTexture() = delete;
	EditRenderTexture(RenderTexture* renderTexture, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept;

private:
	RenderTexture* m_renderTexture;
};
