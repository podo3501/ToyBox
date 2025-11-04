#pragma once

struct IRenderer;
class UIComponent;
class TextureResourceBinder;
class UINameGenerator;
class UIModule;

//UIComponent클래스에서 public에 관한 것들만 여기에서 정의한다. 
// 함수가 UIComponent에 private함수를 을 써야 한다면 이 클래스에서 정의한다.
// protected 안에 함수가 다중책임일 지라도 여기에 옮기지 않는다. 캡슐화가 깨지는게 더 문제이기 때문이다.
class UIComponentEx
{
public:
	UIComponentEx(UIComponent* component) noexcept;

	unique_ptr<UIComponent> AttachComponent(unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent() noexcept;
	unique_ptr<UIComponent> AttachComponent(const string& region, const string& name,
		unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;

	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(const string& region, const string& name) noexcept;

	bool Rename(const string& name) noexcept;
	bool RenameRegion(const string& region) noexcept;

	UIComponent* FindComponent(const string& name) noexcept;
	UIComponent* FindComponent(const string& region, const string& name) noexcept;

	template<typename T>
	T FindComponent(const string& name) noexcept
	{
		UIComponent* find = FindComponent(name);
		return ComponentCast<T>(find);
	}

	UIComponent* FindRegionComponent(const string& regionName) noexcept;
	vector<UIComponent*> FindRenderComponents(const XMINT2& pos) noexcept;
	UIComponent* FindTopRenderComponent(const XMINT2& pos) noexcept;
	XMUINT2 GetChildrenBoundsSize() const noexcept;

private:
	unique_ptr<UIComponent> DetachChild(UIComponent* parent, UIComponent* detach) noexcept;
	UIModule* GetUIModule(UIComponent* start) const;
	UIModule* GetUIModule() const noexcept;
	UINameGenerator* GetNameGenerator() const noexcept;
	void InvalidateUIModuleCache();
	void AssignNamesInRegion(UIComponent* component, UINameGenerator* nameGen, const string& region) noexcept;

	UIComponent* m_component;
	mutable UIModule* m_cachedUIModule;
};

//public 함수들끼리 조합이 가능하면 여기서 처리한다.
bool ChangeSizeX(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept;
bool ChangeSizeY(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept;

//Setup함수가 있어야 하며 인자가 일치 해야 한다. Args는 암묵적 캐스팅이 되지 않는다.
template<typename T, typename... Args>
unique_ptr<T> CreateComponent(Args&&... args)
{
	auto obj = make_unique<T>();
	return obj && obj->Setup(forward<Args>(args)...) ? move(obj) : nullptr;
}