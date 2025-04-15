#pragma once
#include "../../UIComponent.h"

struct TextureSourceInfo;
class PatchTextureLite : public UIComponent
{
public:
	~PatchTextureLite();	
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size = {}) = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	virtual bool FitToTextureSource() noexcept = 0;

	inline optional<TextureSlice> GetTextureSlice() const noexcept { return m_texSlice; }

protected:
	PatchTextureLite();
	PatchTextureLite(TextureSlice texSlice) noexcept;
	PatchTextureLite(const PatchTextureLite& other) noexcept;

private:
	optional<TextureSlice> m_texSlice{ nullopt };
};

unique_ptr<PatchTextureLite> CreatePatchTextureLite(TextureSlice texSlice);