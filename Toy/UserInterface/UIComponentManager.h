#pragma once

struct IRenderer;
class BaseTraverser;
class NameTraverser;
class UILayout;
class UIModul2;
class UIComponentManager
{
public:
	~UIComponentManager();
	UIComponentManager();

	NameTraverser* GetNameTraverser() { return m_nameTraverser.get(); }
	BaseTraverser* GetBaseTraverser() { return m_baseTraverser.get(); }

	UIModul2* CreateUIModule(const UILayout& layout, const string& mainUIName,
		IRenderer* renderer, const wstring& srcBinderFilename = L"");

private:
	unique_ptr<BaseTraverser> m_baseTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;

	unique_ptr<UIModul2> m_uiModule; //지금은 하나이지만 나중에 이게 많아질 수 있다 그때 map으로 교체해야한다.
};