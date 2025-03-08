#include "pch.h"
#include "EditRenderTexture.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"

EditRenderTexture::~EditRenderTexture() = default;
EditRenderTexture::EditRenderTexture(RenderTexture* renderTexture, IRenderer* renderer, CommandList* cmdList) noexcept :
	EditWindow{ renderTexture, cmdList },
	m_renderTexture{ renderTexture }
{}