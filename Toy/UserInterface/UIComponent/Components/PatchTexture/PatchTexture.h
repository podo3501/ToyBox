#pragma once
#include "../../UIComponent.h"

struct ITextureController;
struct TextureSourceInfo;
class PatchTexture : public UIComponent
{
public:
	~PatchTexture();
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept = 0;
	virtual const string& GetBindKey() const noexcept = 0;

	inline TextureSlice GetTextureSlice() const noexcept { return m_texSlice; }
	
protected:
	PatchTexture();
	PatchTexture(TextureSlice texSlice) noexcept;
	PatchTexture(const PatchTexture& other) noexcept;
	void SetTextureSlice(TextureSlice texSlice) noexcept;

private:
	TextureSlice m_texSlice;
};

unique_ptr<PatchTexture> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);