#pragma once

struct IRenderer;
class BaseTraverser;
class DerivedTraverser;
class NameTraverser;
class UILayout;
class UIModul2;
class UIComponentManager
{
public:
	~UIComponentManager();
	UIComponentManager();
	//?!? UI를 로딩하고 Release 해야 두번 로딩이 일어나지 않는다 그러기 위해서는 레퍼런스 카운터를 달아서 몇번 로딩되었는지 체크해서 지워야 한다.
	UIModul2* CreateUIModule(const string& moduleName, const UILayout& layout,
		const string& mainUIName, IRenderer* renderer, const wstring& srcBinderFilename = L"");
	UIModul2* CreateUIModule(const string& moduleName, const wstring& filename,
		IRenderer* renderer, const wstring& srcBinderFilename = L"");
	bool ReleaseUIModule(const string& moduleName) noexcept;
	
	BaseTraverser* GetBaseTraverser() noexcept { return m_baseTraverser.get(); }
	DerivedTraverser* GetDerivedTraverser() noexcept { return m_derivedTraverser.get(); }
	NameTraverser* GetNameTraverser() noexcept { return m_nameTraverser.get(); }

private:
	unique_ptr<BaseTraverser> m_baseTraverser;
	unique_ptr<DerivedTraverser> m_derivedTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;
	unordered_map<string, unique_ptr<UIModul2>> m_uiModules;
};