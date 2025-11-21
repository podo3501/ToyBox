#pragma once

struct IRenderer;
struct ITextureController;
struct ITextureRender;
class BaseTraverser;
class DerivedTraverser;
class NameTraverser;
class UILayout;
class UIModule;
class UIComponent;
class UIComponentManager
{
public:
	~UIComponentManager();
	UIComponentManager() = delete;
	explicit UIComponentManager(IRenderer* renderer, bool isTool = false);
	//?!? UI를 로딩하고 Release 해야 두번 로딩이 일어나지 않는다 그러기 위해서는 레퍼런스 카운터를 달아서 몇번 로딩되었는지 체크해서 지워야 한다.
	UIModule* CreateUIModule(const string& moduleName, const UILayout& layout, const string& mainUIName, const wstring& srcBinderFilename = L"");
	UIModule* CreateUIModule(const string& moduleName, const wstring& filename, const wstring& srcBinderFilename = L"");
	bool ReleaseUIModule(const string& moduleName) noexcept;
	
	inline BaseTraverser* GetBaseTraverser() noexcept { return m_baseTraverser.get(); }
	inline DerivedTraverser* GetDerivedTraverser() noexcept { return m_derivedTraverser.get(); }
	inline NameTraverser* GetNameTraverser() noexcept { return m_nameTraverser.get(); }

	bool CreateRenderTexture(UIComponent* c, const Rectangle& targetRect, size_t& outIndex, UINT64* outGfxMemOffset);
	bool ReleaseRenderTexture(size_t index) noexcept;

private:
	void RenderComponent(ITextureRender* render);
	void RenderTextureComponent(size_t index, ITextureRender* render);

	IRenderer* m_renderer{ nullptr };
	ITextureController* m_texController{ nullptr };
	unique_ptr<BaseTraverser> m_baseTraverser;
	unique_ptr<DerivedTraverser> m_derivedTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;
	map<size_t, UIComponent*> m_renderTextures; //module보다 늦게 삭제되어야 한다.
	unordered_map<string, unique_ptr<UIModule>> m_uiModules;
};