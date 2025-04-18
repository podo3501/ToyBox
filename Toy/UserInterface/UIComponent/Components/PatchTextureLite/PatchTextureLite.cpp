#include "pch.h"
#include "PatchTextureLite.h"
#include "PatchTextureLite1.h"
#include "PatchTextureLite3.h"
#include "PatchTextureLite9.h"

PatchTextureLite::~PatchTextureLite() = default;
PatchTextureLite::PatchTextureLite() = default;
PatchTextureLite::PatchTextureLite(const PatchTextureLite& other) noexcept :
	PatchTextureBase{ other }
{}

unique_ptr<PatchTextureLite> CreatePatchTextureLite(TextureSlice texSlice)
{
	switch (texSlice) {
	case TextureSlice::One: return make_unique<PatchTextureLite1>();
	case TextureSlice::ThreeH: return make_unique<PatchTextureLite3>(DirectionType::Horizontal);
	case TextureSlice::ThreeV: return make_unique<PatchTextureLite3>(DirectionType::Vertical);
	case TextureSlice::Nine: return make_unique<PatchTextureLite9>();
	}
	return nullptr;
}