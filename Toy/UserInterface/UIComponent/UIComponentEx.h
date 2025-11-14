//#pragma once
//
//struct IRenderer;
//struct IMouseEventReceiver;
//class UIComponent;
//class TextureResourceBinder;
//class UINameGenerator;
//class UIModule;
//
////UIComponent클래스에서 public에 관한 것들만 여기에서 정의한다. 
//// 함수가 UIComponent에 private함수를 을 써야 한다면 이 클래스에서 정의한다.
//// protected 안에 함수가 다중책임일 지라도 여기에 옮기지 않는다. 캡슐화가 깨지는게 더 문제이기 때문이다.
//class UIComponentEx
//{
//public:
//	UIComponentEx(UIComponent* component) noexcept;
//
//	unique_ptr<UIComponent> AttachComponent(unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept;
//	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent() noexcept;
//	unique_ptr<UIComponent> AttachComponent(const string& region, const string& name,
//		unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept;
//
//	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(const string& region, const string& name) noexcept;
//
//	bool Rename(const string& name) noexcept;
//	bool RenameRegion(const string& region) noexcept;
//
//	UIComponent* FindComponent(const string& name) noexcept;
//	UIComponent* FindComponent(const string& region, const string& name) noexcept;
//
//	template<typename T>
//	T FindComponent(const string& name) noexcept
//	{
//		UIComponent* find = FindComponent(name);
//		return ComponentCast<T>(find);
//	}
//
//	UIComponent* FindRegionComponent(const string& regionName) noexcept;
//	vector<UIComponent*> PickComponents(const XMINT2& pos) noexcept;
//	vector<IMouseEventReceiver*> PickMouseReceivers(const XMINT2& pos) noexcept;
//	XMUINT2 GetChildrenBoundsSize() const noexcept;
//
//private:
//	unique_ptr<UIComponent> DetachChild(UIComponent* parent, UIComponent* detach) noexcept;
//	UIModule* GetUIModule(UIComponent* start) const;
//	UIModule* GetUIModule() const noexcept;
//	UINameGenerator* GetNameGenerator() const noexcept;
//	void InvalidateUIModuleCache();
//	void AssignNamesInRegion(UIComponent* component, UINameGenerator* nameGen, const string& region) noexcept;
//	bool RemoveAndMergeRegion(UIComponent* c, UIComponent* parentRoot, 
//		UINameGenerator* nameGen, const string& oldRegion) noexcept;
//	bool ReplaceAndMergeRegion(UIComponent* c, UIComponent* parentRoot, 
//		UINameGenerator* nameGen, const string& oldRegion, const string& newRegion) noexcept;
//
//	UIComponent* m_component;
//	mutable UIModule* m_cachedUIModule;
//};