#pragma once
#include "../../UIComponent.h"

struct ITextureController;
class PatchTexture : public UIComponent
{
public:
	~PatchTexture();
	PatchTexture();
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	
protected:
	PatchTexture(const PatchTexture& other);
};

unique_ptr<PatchTexture> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);