#include "pch.h"
#include "PatchTextureStd.h"
#include "PatchTextureStd1.h"
#include "PatchTextureStd3.h"
#include "PatchTextureStd9.h"

PatchTextureStd::~PatchTextureStd() = default;
PatchTextureStd::PatchTextureStd() = default;
PatchTextureStd::PatchTextureStd(const PatchTextureStd& other) noexcept :
	PatchTexture{ other }
{}

const string& PatchTextureStd::GetBindKey() const noexcept
{
	PatchTextureStd* child = static_cast<PatchTextureStd*>(GetChildComponent(0));
	return child->GetBindKey();
}

bool PatchTextureStd::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	if (GetSize() == XMUINT2{})
		return ArrangeTextures();

	return ChangeSize(GetSize(), true);
}

//////////////////////////////////////////////////////////

unique_ptr<PatchTextureStd> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey)
{
	UILayout ltLayout({ layout.GetSize(), Origin::LeftTop });
	switch (texSlice) {
	case TextureSlice::One: return CreatePatchTextureStd1(ltLayout, bindKey);
	case TextureSlice::ThreeH: return CreatePatchTextureStd3(ltLayout, DirectionType::Horizontal, bindKey);
	case TextureSlice::ThreeV: return CreatePatchTextureStd3(ltLayout, DirectionType::Vertical, bindKey);
	case TextureSlice::Nine: return CreatePatchTextureStd9(ltLayout, bindKey);
	}
	return nullptr;
}