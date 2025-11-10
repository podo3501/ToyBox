#pragma once
#include "HierarchyTraverser.h"

class UIComponent;
class UINameGenerator;
class AttachmentTraverser : private HierarchyTraverser
{
public:
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;

private:
};

