#pragma once
#include "../Include/TypeAliases.h"
#include "../Include/ITextureBinder.h"
#include "TextureSourceInfo.h"

struct ITextureLoad;
class TextureLoadBinder : public ITextureBinder
{
public:
	virtual bool LoadResources(ITextureLoad* load) override;

public:
	void AddTexture(const wstring& filename) noexcept;
	optionalRef<TextureSourceInfo> GetSourceInfo(const wstring& filename) const noexcept;

private:
	auto FindTextureByFilename(const wstring& filename) const noexcept;

	vector<TextureSourceInfo> m_textures;
};
