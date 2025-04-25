#pragma once
#include "../PatchTexture.h"

struct ITextureController;
class TextureResourceBinder;
class PatchTextureStd : public PatchTexture
{
public:
	~PatchTextureStd();
	virtual bool ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept = 0;
	virtual const string& GetBindKey() const noexcept;

protected:
	using PatchTexture::PatchTexture;
	PatchTextureStd();
	PatchTextureStd(const PatchTextureStd& other) noexcept;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
};

unique_ptr<PatchTextureStd> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);