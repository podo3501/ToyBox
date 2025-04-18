#pragma once
#include "../PatchTexture/PatchTexture.h"

struct TextureSourceInfo;
class PatchTextureLite : public PatchTextureBase
{
public:
	~PatchTextureLite();	
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size = {}) = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	virtual bool FitToTextureSource() noexcept = 0;

protected:
	using PatchTextureBase::PatchTextureBase;
	PatchTextureLite();
	PatchTextureLite(const PatchTextureLite& other) noexcept;
};

unique_ptr<PatchTextureLite> CreatePatchTextureLite(TextureSlice texSlice);