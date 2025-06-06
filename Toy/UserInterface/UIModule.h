#pragma once

struct IRenderer;
struct ITextureRender;
class UILayout;
class UIComponent;
class UINameGenerator;
class TextureResourceBinder;
class JsonOperation;
namespace DX { class StepTimer; }
class UIModule
{
public:
	~UIModule();
	UIModule() noexcept;
	bool operator==(const UIModule& other) const noexcept;

	bool SetupMainComponent(const UILayout& layout, const string& name,
		IRenderer* renderer, const wstring& srcBinderFilename);
	bool SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
	void AddRenderer() noexcept;
	bool BindTextureResources() noexcept;
	bool Update(const DX::StepTimer& timer) noexcept;
	void Render(ITextureRender* render) const;
	unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	unique_ptr<UIComponent> AttachComponent(const string& regionName, const string& name,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* component) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(const string& regionName, const string& name) noexcept;
	void SerializeIO(JsonOperation& operation);
	void Rename(UIComponent* component, const string& name);
	bool Write(const wstring& filename = L"") noexcept;
	bool EnableToolMode(bool enable);
	UIComponent* FindComponent(const string& name) const noexcept;
	inline UIComponent* GetComponent() const noexcept { return m_component.get(); }
	inline TextureResourceBinder* GetTexResBinder() const noexcept { return m_resBinder.get(); }
	inline const wstring& GetFilename() const noexcept { return m_filename; }

private:
	bool Read(const wstring& filename = L"") noexcept;
	UIComponent* FindComponentInRegion(const string& regionName, const string& name) const noexcept;

	unique_ptr<UINameGenerator> m_generator;
	unique_ptr<UIComponent> m_component;
	unique_ptr<TextureResourceBinder> m_resBinder;
	IRenderer* m_renderer;

	wstring m_filename{};
};

unique_ptr<UIModule> CreateUIModule(const UILayout& layout, const string& mainUIName,
	IRenderer* renderer, const wstring& srcBinderFilename);
unique_ptr<UIModule> CreateUIModule(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
