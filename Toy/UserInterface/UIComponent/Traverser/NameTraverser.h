#pragma once
#include "HierarchyTraverser.h"

class UIComponent;
class UINameGenerator;
class NameTraverser : private HierarchyTraverser
{
public:
	NameTraverser(UINameGenerator* nameGen);
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c) noexcept;
	UIComponent* FindComponent(UIComponent* c, const string& name) noexcept;
	UIComponent* FindRegionComponent(UIComponent* c, const string& region) noexcept;
	bool Rename(UIComponent* c, const string& name) noexcept;
	bool RenameRegion(UIComponent* component, const string& region) noexcept;

private:
	void AssignNamesInRegion(UIComponent* component, const string& region) noexcept;
	bool RemoveAndMergeRegion(UIComponent* c, UIComponent* parentRoot, const string& oldRegion) noexcept;
	bool ReplaceAndMergeRegion(UIComponent* c, UIComponent* parentRoot, const string& oldRegion, const string& newRegion) noexcept;

	UINameGenerator* m_nameGen{ nullptr };
};
