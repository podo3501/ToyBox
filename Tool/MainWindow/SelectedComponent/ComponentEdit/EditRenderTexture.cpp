#include "pch.h"
#include "EditRenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"

EditRenderTexture::~EditRenderTexture() = default;
EditRenderTexture::EditRenderTexture(RenderTexture* renderTexture, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
	EditWindow{ renderTexture, sourceBinder, cmdList },
	m_renderTexture{ renderTexture }
{}