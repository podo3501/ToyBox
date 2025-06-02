#pragma once

struct IRenderer;
class UILayout;
class UIComponent;
class UINameGenerator;
class TextureResourceBinder;
class JsonOperation;
class UIModule
{
public:
	~UIModule();
	UIModule() noexcept;
	bool operator==(const UIModule& other) const noexcept;

	bool SetupMainComponent(const UILayout& layout, const string& name,
		IRenderer* renderer, const wstring& srcBinderFilename);
	bool SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
	bool BindTextureResources() noexcept;
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* component) noexcept;
	void SerializeIO(JsonOperation& operation);
	void Rename(UIComponent* component, const string& name);
	bool Write(const wstring& filename = L"") noexcept;
	inline UIComponent* GetComponent() const noexcept { return m_component.get(); }
	inline TextureResourceBinder* GetTexResBinder() const noexcept { return m_resBinder.get(); }

private:
	bool Read(const wstring& filename = L"") noexcept;

	unique_ptr<UINameGenerator> m_generator;
	unique_ptr<UIComponent> m_component;
	unique_ptr<TextureResourceBinder> m_resBinder;
	IRenderer* m_renderer;

	wstring m_filename{};
};

unique_ptr<UIModule> CreateUIModule(const UILayout& layout, const string& mainUIName,
	IRenderer* renderer, const wstring& srcBinderFilename);
unique_ptr<UIModule> CreateUIModule(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
