#pragma once
#include "HierarchyTraverser.h"

struct IMouseEventReceiver;
class UIComponent;
class BaseTraverser : private HierarchyTraverser
{
public:
	XMUINT2 GetChildrenBoundsSize(UIComponent* c) const noexcept;
	vector<UIComponent*> PickComponents(UIComponent* c, const XMINT2& pos) noexcept;
	vector<IMouseEventReceiver*> PickMouseReceivers(UIComponent* c, const XMINT2& pos) noexcept;
	string GetMyRegion(UIComponent* c) const noexcept;

private:
};

