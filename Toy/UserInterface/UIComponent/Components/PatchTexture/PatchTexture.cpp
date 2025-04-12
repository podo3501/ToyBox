#include "pch.h"
#include "PatchTexture.h"
#include "PatchTexture1.h"
#include "PatchTexture3.h"
#include "PatchTexture9.h"

PatchTexture::~PatchTexture() = default;
PatchTexture::PatchTexture() = default;
PatchTexture::PatchTexture(TextureSlice texSlice) noexcept :
	m_texSlice{ texSlice }
{}

PatchTexture::PatchTexture(const PatchTexture& other) noexcept :
	UIComponent{ other },
	m_texSlice{ other.m_texSlice }
{}

void PatchTexture::SetTextureSlice(TextureSlice texSlice) noexcept
{
	m_texSlice = texSlice;
}

//////////////////////////////////////////////////////////

unique_ptr<PatchTexture> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey)
{
	UILayout ltLayout({ layout.GetSize(), Origin::LeftTop });
	switch (texSlice) {
	case TextureSlice::One: return CreateUnboundPatchTexture1(ltLayout);
	case TextureSlice::ThreeH: return CreatePatchTexture3(ltLayout, DirectionType::Horizontal, bindKey);
	case TextureSlice::ThreeV: return CreatePatchTexture3(ltLayout, DirectionType::Vertical, bindKey);
	case TextureSlice::Nine: return CreatePatchTexture9(ltLayout, bindKey);
	}
	return nullptr;
}