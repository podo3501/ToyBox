#include "pch.h"
#include "EditRenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"

EditRenderTexture::~EditRenderTexture() = default;
EditRenderTexture::EditRenderTexture(RenderTexture* renderTexture, UICommandList* cmdList) noexcept :
	EditWindow{ renderTexture, cmdList },
	m_renderTexture{ renderTexture }
{}