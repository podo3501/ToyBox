#include "pch.h"
#include "TextureInfo.h"
#include "../Include/IRenderer.h"

TextureInfo::~TextureInfo()
{
    Release();
}

TextureInfo::TextureInfo() noexcept :
    m_texLoader{ nullptr }
{}

TextureInfo::TextureInfo(const TextureInfo& other) noexcept : 
    m_texLoader{ other.m_texLoader },
    m_index{ other.m_index },
    m_gfxOffset{ other.m_gfxOffset }
{
    AddRef();
}

TextureInfo::TextureInfo(TextureInfo&& other) noexcept :
    m_texLoader{ move(other.m_texLoader) },
    m_index{ move(other.m_index) },
    m_gfxOffset{ move(other.m_gfxOffset) }
{
    AddRef();
}

void TextureInfo::AddRef() noexcept
{
    if (m_texLoader && m_index)
        m_texLoader->AddRef(*m_index);
}

void TextureInfo::Release() noexcept
{
    if (m_texLoader && m_index)
    {
        m_texLoader->ReleaseTexture(*m_index);
        m_texLoader = nullptr;
        m_index = nullopt;
    }
}