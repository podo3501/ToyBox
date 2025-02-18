#include "pch.h"
#include "UIHierarchy.h"
#include "UIComponent.h"
#include "UIComponentEx.h"
#include "../Utility.h"

UIComponent* UIHierarchy<UIComponent>::GetThis() noexcept
{
	return static_cast<UIComponent*>(this);
}

UIComponent* UIHierarchy<UIComponent>::GetRoot() noexcept
{
	UIComponent* current = GetThis();
	while (current->m_parent != nullptr)
		current = current->m_parent;

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetRegionRoot() noexcept
{
	UIComponent* current = GetThis();
	while (current->GetRegion().empty())
	{
		if (!current->m_parent) break;
		current = current->m_parent;
	}

	if (current->GetRegion().empty())
		return nullptr;

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetParentRegionRoot() noexcept
{
	if (m_parent == nullptr) return GetThis();
	return m_parent->GetRegionRoot();
}

void UIHierarchy<UIComponent>::ForEachChild(function<void(UIComponent*)> func) noexcept
{
	func(GetThis());
	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChild(func);
	}
}

bool UIHierarchy<UIComponent>::ForEachChildUntilFail(function<bool(UIComponent*)> func) noexcept
{
	ReturnIfFalse(func(GetThis()));

	for (auto& child : m_children) 
		if (child) ReturnIfFalse(child->ForEachChildUntilFail(func));

	return true;
}

void UIHierarchy<UIComponent>::ForEachChildBool(function<bool(UIComponent*)> func) noexcept
{
	if (!func(GetThis()))
		return;

	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChildBool(func);
	}
}

void UIHierarchy<UIComponent>::ForEachChildConst(function<void(const UIComponent*)> func) const noexcept
{
	func(static_cast<const UIComponent*>(this));
	for (const auto& child : m_children)
	{
		if (child)
			child->ForEachChildConst(func);
	}
}

void UIHierarchy<UIComponent>::ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> func) noexcept
{
	queue<UIComponent*> queue;
	queue.push(GetThis());

	while (!queue.empty())
	{
		UIComponent* current = queue.front();
		queue.pop();

		//if (!current->m_enable) continue;
		if (!current->HasStateFlag(flag)) continue;

		func(current);

		for (const auto& child : current->m_children)
		{
			if (child)
				queue.push(child.get());
		}
	}
}

//�θ� region���� ���� �̸��� �ִ��� Ȯ���Ѵ�.
bool UIHierarchy<UIComponent>::IsUniqueRegion(const string& name) noexcept
{
	UIComponent* regionRoot = GetParentRegionRoot();
	if (!regionRoot) return false;

	if (UIEx(regionRoot).GetRegionComponent(name)) return false;

	return true;
}

//���� �̸��� _���� �� �پ������� �̸��� _�� ���� �ƴϸ� Ÿ���̸���_�� �ٿ��� ����
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
	//�ٴ� Component���� region�� ������ �ֱ� ������ ���� �� Component�� �´� �θ� regionRoot�� ã�´�.
	auto& uiComponentEx = self->GetRegionRoot() ? self->GetUIComponentEx() : GetThis()->GetUIComponentEx();
	UIComponent* findComponent = uiComponentEx.GetComponent(name);
	if (findComponent && findComponent != self) return false;

	return true;
}

bool UIHierarchy<UIComponent>::IsUniqueRegionName(const string& name, UIComponent* self) noexcept
{
	//�θ� ���ų� �θ� �־ �θ��� region ��Ʈ�� ���°��. �Ѵܰ� �ö󰡼� RegionRoot���� �̸����� ã�ƺ��� �Ѵ�.
	auto& uiComponentEx = (self->m_parent && self->m_parent->GetRegionRoot()) ? self->GetUIComponentEx() : GetThis()->GetUIComponentEx();
	UIComponent* findComponent = uiComponentEx.GetRegionComponent(name);
	if (findComponent && findComponent != self) return false;

	return true;
}

string UIHierarchy<UIComponent>::CreateNewName(UIComponent* attaching) noexcept
{
	const string& name = attaching->m_name;
	if (!name.empty() && IsUniqueName(name, attaching)) return name;

	auto [baseName, _] = GetBaseName(name, EnumToString(attaching->GetTypeID()));
	string newName; int n{ 0 };
	do {
		newName = string(baseName) + to_string(n++);
	} while (!IsUniqueName(newName, attaching));

	return newName;
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

void UIHierarchy<UIComponent>::GenerateUniqueName(UIComponent* attachingBlock) noexcept
{
	map<string, UIComponent*> newNames;

	attachingBlock->ForEachChild([this, &newNames](UIComponent* attaching) {
		const string& name = CreateNewName(attaching);
		const string& uniqueName = CreateUniqueName(newNames, name);
		newNames[uniqueName] = attaching;
		});

	for (auto& uniqueName : newNames)
		uniqueName.second->m_name = uniqueName.first;
}

void UIHierarchy<UIComponent>::GenerateUniqueRegionName(UIComponent* attachingBlock) noexcept
{
	map<string, UIComponent*> newNames;

	attachingBlock->ForEachChild([this, &newNames](UIComponent* attaching) {
		const string& name = CreateNewRegionName(attaching);
		string uniqueName;
		if (!name.empty()) uniqueName = CreateUniqueName(newNames, name);
		if (!uniqueName.empty()) newNames[uniqueName] = attaching;
		});

	for (auto& uniqueName : newNames)
		uniqueName.second->m_region = uniqueName.first;
}
