#include "pch.h"
#include "PatchTexture.h"

PatchTexture::~PatchTexture() = default;
PatchTexture::PatchTexture() = default;
PatchTexture::PatchTexture(TextureSlice texSlice) noexcept :
	m_texSlice{ texSlice }
{}

PatchTexture::PatchTexture(const PatchTexture& other) noexcept :
	UIComponent{ other },
	m_texSlice{ other.m_texSlice }
{}