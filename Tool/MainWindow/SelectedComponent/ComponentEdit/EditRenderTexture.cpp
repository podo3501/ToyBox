#include "pch.h"
#include "EditRenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"

EditRenderTexture::~EditRenderTexture() = default;
EditRenderTexture::EditRenderTexture(RenderTexture* renderTexture, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
	EditWindow{ renderTexture, resBinder, cmdList },
	m_renderTexture{ renderTexture }
{}