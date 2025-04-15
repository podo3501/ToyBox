#include "pch.h"
#include "PatchTextureLite.h"
#include "PatchTextureLite1.h"
#include "PatchTextureLite3.h"

PatchTextureLite::~PatchTextureLite() = default;
PatchTextureLite::PatchTextureLite() = default;
PatchTextureLite::PatchTextureLite(TextureSlice texSlice) noexcept :
	m_texSlice{ texSlice }
{}

PatchTextureLite::PatchTextureLite(const PatchTextureLite& other) noexcept :
	UIComponent{ other },
	m_texSlice{ other.m_texSlice }
{}

unique_ptr<PatchTextureLite> CreatePatchTextureLite(TextureSlice texSlice)
{
	switch (texSlice) {
	case TextureSlice::One: return make_unique<PatchTextureLite1>();
	case TextureSlice::ThreeH: return make_unique<PatchTextureLite3>(DirectionType::Horizontal);
	//case TextureSlice::ThreeH: return CreatePatchTexture3(ltLayout, DirectionType::Horizontal, bindKey);
	//	//case TextureSlice::ThreeH: return CreateUnboundPatchTexture3(ltLayout, DirectionType::Horizontal);
	//case TextureSlice::ThreeV: return CreatePatchTexture3(ltLayout, DirectionType::Vertical, bindKey);
	//case TextureSlice::Nine: return CreatePatchTexture9(ltLayout, bindKey);
	}
	return nullptr;
}