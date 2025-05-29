#pragma once

struct IRenderer;
class UILayout;
class UIComponent;
class UINameGenerator;
class UIModule
{
public:
	~UIModule();
	UIModule() noexcept;
	bool operator==(const UIModule& other) const noexcept;

	bool SetupMainComponent(const string& name, const UILayout& layout, IRenderer* renderer);
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* component) noexcept;
	void Rename(UIComponent* component, const string& name);
	bool Write(const wstring& filename = L"") noexcept;
	bool Read(const wstring& filename = L"") noexcept;
	inline UIComponent* GetComponent() const noexcept { return m_component.get(); }

private:
	unique_ptr<UINameGenerator> m_generator;
	unique_ptr<UIComponent> m_component;

	wstring m_filename{};
};

unique_ptr<UIModule> CreateUIModule(const string& mainUIName, const UILayout& layout, IRenderer* renderer) noexcept;
