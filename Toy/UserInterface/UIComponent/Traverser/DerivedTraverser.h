#pragma once
#include "HierarchyTraverser.h"

struct ITextureRender;

class DerivedTraverser : private HierarchyTraverser
{
public:
	void Render(UIComponent* c, ITextureRender* render);
	bool BindTextureSourceInfo(UIComponent* c, TextureResourceBinder* resBinder, ITextureController* texController) noexcept;

private:
};
