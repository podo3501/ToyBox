#pragma once
#include "../PatchTexture.h"

class PatchTextureStd : public PatchTexture
{
public:
	~PatchTextureStd();
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept = 0;
	virtual const string& GetBindKey() const noexcept = 0;

protected:
	using PatchTexture::PatchTexture;
	PatchTextureStd();
	PatchTextureStd(const PatchTextureStd& other) noexcept;
};

unique_ptr<PatchTextureStd> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);