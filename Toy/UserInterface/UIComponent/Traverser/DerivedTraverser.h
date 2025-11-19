#pragma once
#include "HierarchyTraverser.h"

struct ITextureRender;
struct ITextureController;
class TextureResourceBinder;
class DerivedTraverser : private HierarchyTraverser
{
public:
	void Render(UIComponent* c, ITextureRender* render);
	bool BindTextureSourceInfo(UIComponent* c, TextureResourceBinder* resBinder, ITextureController* texController) noexcept;
	void PropagateRoot(UIComponent* c, UIComponent* root) noexcept;
	bool EnableToolMode(UIComponent* c, bool enable) noexcept;
	bool UpdatePositionsManually(UIComponent* c, bool isRoot = false) noexcept;

private:
	bool RecursivePositionUpdate(UIComponent* c, const XMINT2& position = {}) noexcept;
};
