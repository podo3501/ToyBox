#pragma once
#include "../../UIComponent.h"

class PatchTextureLite : public UIComponent
{
public:
	~PatchTextureLite();	
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;

protected:
	PatchTextureLite();
};