#pragma once
#include "HierarchyTraverser.h"

class UIComponent;
class UINameGenerator;
class NameTraverser : private HierarchyTraverser
{
public:
	NameTraverser(UINameGenerator* nameGen);
	UIComponent* FindComponent(UIComponent* c, const string& name) noexcept;
	bool Rename(UIComponent* c, const string& name) noexcept;
	bool RenameRegion(UIComponent* component, const string& region) noexcept;

private:
	void AssignNamesInRegion(UIComponent* component, const string& region) noexcept;
	bool RemoveAndMergeRegion(UIComponent* c, UIComponent* parentRoot, const string& oldRegion) noexcept;
	bool ReplaceAndMergeRegion(UIComponent* c, UIComponent* parentRoot, const string& oldRegion, const string& newRegion) noexcept;

	UINameGenerator* m_nameGen{ nullptr };
};
