#pragma once

class UIComponent;
class UINameGenerator;
class UIRegistry
{
public:
	~UIRegistry();
	UIRegistry() noexcept;

	inline void SetEntryComponent(UIComponent* component) noexcept { m_entryComponent = component; }
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* component) noexcept;
	void Rename(UIComponent* component, const string& name);
	bool Save(const wstring& filename = L"") noexcept;

private:
	unique_ptr<UINameGenerator> m_generator;
	UIComponent* m_entryComponent;
	wstring m_filename{};
};
