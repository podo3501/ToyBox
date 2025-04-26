#pragma once
#include "../PatchTexture.h"

struct ITextureController;
class TextureResourceBinder;
class PatchTextureStd : public PatchTexture
{
public:
	~PatchTextureStd();
	virtual bool ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept;
	virtual const string& GetBindKey() const noexcept;

protected:
	using PatchTexture::PatchTexture;
	PatchTextureStd();
	PatchTextureStd(const PatchTextureStd& other) noexcept;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& info, size_t sourceIndex) noexcept;
};

unique_ptr<PatchTextureStd> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);