#include "pch.h"
#include "TextureInfo.h"
#include "../Include/IRenderer.h"

TextureInfo::TextureInfo() noexcept :
    m_texLoader{ nullptr }
{}

void TextureInfo::Release() noexcept
{
    if (m_texLoader && m_index)
    {
        m_texLoader->ReleaseTexture(*m_index);
        m_texLoader = nullptr;
        m_index = nullopt;
    }
}