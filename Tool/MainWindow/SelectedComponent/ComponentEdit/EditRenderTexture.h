#pragma once
#include "EditWindow.h"

class RenderTexture;
class EditRenderTexture : public EditWindow
{
public:
	~EditRenderTexture();
	EditRenderTexture() = delete;
	EditRenderTexture(RenderTexture* renderTexture, IRenderer* renderer, CommandList* cmdList) noexcept;

private:
	RenderTexture* m_renderTexture;
};
