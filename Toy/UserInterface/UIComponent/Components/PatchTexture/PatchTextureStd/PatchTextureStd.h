#pragma once
#include "../PatchTexture.h"

class TextureResourceBinder;
class PatchTextureStd : public PatchTexture
{
public:
	~PatchTextureStd();
	virtual bool ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept = 0;
	virtual const string& GetBindKey() const noexcept = 0;

protected:
	using PatchTexture::PatchTexture;
	PatchTextureStd();
	PatchTextureStd(const PatchTextureStd& other) noexcept;
};

unique_ptr<PatchTextureStd> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);