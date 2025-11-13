#pragma once
#include "HierarchyTraverser.h"

class UIComponent;
class UINameGenerator;
class NameTraverser : private HierarchyTraverser //Traverser은 상태값을 가질 수 없다.
{
public:
	NameTraverser();
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c) noexcept;
	UIComponent* FindComponent(UIComponent* c, const string& name) noexcept;
	UIComponent* FindRegionComponent(UIComponent* c, const string& region) noexcept;
	bool Rename(UIComponent* c, const string& name) noexcept;
	bool RenameRegion(UIComponent* component, const string& region) noexcept;

private:
	UINameGenerator* GetNameGenerator(UIComponent* c) noexcept;
	void AssignNamesInRegion(UINameGenerator* nameGen, UIComponent* component, const string& region) noexcept;
	bool RemoveAndMergeRegion(UINameGenerator* nameGen, UIComponent* c, 
		UIComponent* parentRoot, const string& oldRegion) noexcept;
	bool ReplaceAndMergeRegion(UINameGenerator* nameGen, UIComponent* c, 
		UIComponent* parentRoot, const string& oldRegion, const string& newRegion) noexcept;
};
