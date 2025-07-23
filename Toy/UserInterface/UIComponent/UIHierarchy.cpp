#include "pch.h"
#include "UIHierarchy.h"
#include "UIComponent.h"
#include "UIComponentEx.h"
#include "Utility.h"

UIComponent* UIHierarchy<UIComponent>::GetThis() const noexcept
{
	UIHierarchy<UIComponent>* hierarchy = const_cast<UIHierarchy<UIComponent>*>(this);
	return static_cast<UIComponent*>(hierarchy);
}

UIComponent* UIHierarchy<UIComponent>::GetRoot() const noexcept
{
	UIComponent* current = GetThis();
	while (current->m_parent != nullptr)
		current = current->m_parent;

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetRegionRoot() const noexcept
{
	UIComponent* current = GetThis();
	while (current->GetRegion().empty())
	{
		if (!current->m_parent) break;
		current = current->m_parent;
	}

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetParentRegionRoot() const noexcept //?!? 자기껀 검사 안하고 부모로 올라가는데 그렇다면 이 함수가 쓰여야 할까?
{
	if (m_parent == nullptr) return GetThis();
	return m_parent->GetRegionRoot();
}

void UIHierarchy<UIComponent>::ForEachChildBool(function<CResult(UIComponent*)> Func) noexcept
{
	if (Func(GetThis()) == CResult::SkipChildren)
		return;

	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChildBool(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachRenderChildBFS(function<void(UIComponent*)> Func) noexcept
{
	queue<UIComponent*> cQueue;
	auto PushChild = [&cQueue](UIComponent* c) { if (c->HasStateFlag(StateFlag::Render)) cQueue.push(c); };

	PushChild(GetThis());

	while (!cQueue.empty())
	{
		UIComponent* current = cQueue.front();
		cQueue.pop();

		if (current->GetRenderSearchType() == RenderTraversal::DFS)
		{
			current->ForEachRenderChildDFS(Func);
			continue;
		}

		Func(current);
		if (current->HasStateFlag(StateFlag::RenderTexture)) continue;

		for (const auto& child : current->m_children)
		{
			if (!child) continue;
			PushChild(child.get());
		}
	}
}

template<typename T>
void UIHierarchy<T>::ForEachRenderChildDFS(function<void(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	Func(current);
	if (current->HasStateFlag(StateFlag::RenderTexture)) return;

	for (auto& child : m_children)
	{
		if (!child) continue;
		child->ForEachRenderChildDFS(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachChildToRender(function<void(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	RenderTraversal traversal = current->GetRenderSearchType();
	if(traversal == RenderTraversal::BFS || traversal == RenderTraversal::Inherited )
		return ForEachRenderChildBFS(Func);

	return ForEachRenderChildDFS(Func);
}

void UIHierarchy<UIComponent>::ForEachChildWithRegion(function<void(const string&, UIComponent*)> Func) noexcept
{
	const auto Traverse = [&](UIHierarchy<UIComponent>* node, const string& region, auto&& self_ref) -> void {
		UIComponent* component = static_cast<UIComponent*>(node);
		string currentRegion = !component->GetRegion().empty() ? component->GetRegion() : region;
		Func(currentRegion, component);

		for (auto& child : node->m_children) {
			if (child)
				self_ref(child.get(), currentRegion, self_ref);
		}};

	UIComponent* regionComponent = GetParentRegionRoot();
	Traverse(GetThis(), regionComponent->GetRegion(), Traverse);
}

void UIHierarchy<UIComponent>::ForEachChildInSameRegion(function<void(UIComponent*)> Func) noexcept
{
	const auto Traverse = [&](UIHierarchy<UIComponent>* node, const string& region, auto&& self_ref) -> void {
		UIComponent* component = static_cast<UIComponent*>(node);

		const string& nodeRegion = component->GetRegion();
		if (!nodeRegion.empty() && nodeRegion != region)
			return;

		Func(component);

		for (auto& child : node->m_children) {
			if (child)
				self_ref(child.get(), region, self_ref);
		}};

	UIComponent* regionComponent = GetRegionRoot();
	Traverse(GetThis(), regionComponent->GetRegion(), Traverse);
}

//부모 region에서 같은 이름이 있는지 확인한다.
bool UIHierarchy<UIComponent>::IsUniqueRegion(const string& name) noexcept
{
	UIComponent* regionRoot = GetParentRegionRoot();
	if (!regionRoot) return false;

	if (UIEx(regionRoot).GetRegionComponent(name)) return false;

	return true;
}

//기존 이름에 _숫자 가 붙어있으면 이름에 _를 리턴 아니면 타입이름에_를 붙여서 리턴
static pair<string, int> GetBaseName(const string& name, const string& base = "Unknown")
{
	if (name.empty()) return { base + "_", 0 };

	size_t pos = name.rfind('_');
	string baseName = (pos == string::npos) ? name + "_" : name.substr(0, pos + 1);
	string afterUnderline = (pos == string::npos) ? "non-numeric" : name.substr(pos + 1);

	int index = (ranges::all_of(afterUnderline, ::isdigit)) ? stoi(afterUnderline) + 1 : 0;
	return { baseName, index };
}

bool UIHierarchy<UIComponent>::IsUniqueName(const string& name, UIComponent* self) noexcept
{
	//붙는 Component에도 region이 있을수 있기 때문에 먼저 이 Component에 맞는 부모 regionRoot를 찾는다.
	auto& uiComponentEx = self->GetRegionRoot() ? self->GetUIComponentEx() : GetThis()->GetUIComponentEx();
	UIComponent* findComponent = uiComponentEx.FindComponent(name);
	if (findComponent && findComponent != self) return false;

	return true;
}

bool UIHierarchy<UIComponent>::IsUniqueRegionName(const string& name, UIComponent* self) noexcept
{
	//부모가 없거나 부모가 있어도 부모의 region 루트가 없는경우. 한단계 올라가서 RegionRoot에서 이름값을 찾아봐야 한다.
	auto& uiComponentEx = (self->m_parent && self->m_parent->GetRegionRoot()) ? self->GetUIComponentEx() : GetThis()->GetUIComponentEx();
	UIComponent* findComponent = uiComponentEx.GetRegionComponent(name);
	if (findComponent && findComponent != self) return false;

	return true;
}

string UIHierarchy<UIComponent>::CreateNewRegionName(UIComponent* attaching) noexcept
{
	const string& name = attaching->m_region;
	if (name.empty()) return name;
	if (IsUniqueRegionName(name, attaching)) return name;

	auto [baseName, _] = GetBaseName(name);
	string newName; int n{ 0 };
	do {
		newName = string(baseName) + to_string(n++);
	} while (!IsUniqueRegionName(newName, attaching));

	return newName;
}

static string CreateUniqueName(const map<string, UIComponent*>& names, const string& createName)
{
	auto IsExistName = [&names](const string& name)->bool { auto find = names.find(name); return find != names.end(); };

	if (!IsExistName(createName)) return createName;

	auto [baseName, startIdx] = GetBaseName(createName);
	string newName;
	do {
		newName = string(baseName) + to_string(startIdx++);
	} while (IsExistName(newName));

	return newName;
}

void UIHierarchy<UIComponent>::GenerateUniqueRegionName(UIComponent* attachingBlock) noexcept
{
	map<string, UIComponent*> newNames;

	attachingBlock->ForEachChildDFS([this, &newNames](UIComponent* attaching) {
		const string& name = CreateNewRegionName(attaching);
		string uniqueName;
		if (!name.empty()) uniqueName = CreateUniqueName(newNames, name);
		if (!uniqueName.empty()) newNames[uniqueName] = attaching;
		});

	for (auto& uniqueName : newNames)
		uniqueName.second->m_region = uniqueName.first;
}
