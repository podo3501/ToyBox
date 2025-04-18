#pragma once

struct IRenderer;
class UIComponent;
class TextureResourceBinder;

//UIComponent클래스에서 public에 관한 것들만 여기에서 정의한다. 
// 함수가 UIComponent에 private함수를 을 써야 한다면 이 클래스에서 정의한다.
// protected 안에 함수가 다중책임일 지라도 여기에 옮기지 않는다. 캡슐화가 깨지는게 더 문제이기 때문이다.
class UIComponentEx
{
public:
	UIComponentEx(UIComponent* component) noexcept;

	unique_ptr<UIComponent> AttachComponent(unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent() noexcept;

	UIComponent* FindComponent(const string& name) noexcept;

	template<typename T>
	T FindComponent(const string& name) noexcept
	{
		UIComponent* find = FindComponent(name);
		return ComponentCast<T>(find);
	}

	UIComponent* GetRegionComponent(const string& regionName) noexcept;
	vector<UIComponent*> GetComponents(const XMINT2& pos) noexcept;
	XMUINT2 GetChildrenBoundsSize() const noexcept;
	bool IsPositionUpdated() const noexcept;

private:
	unique_ptr<UIComponent> DetachChild(UIComponent* parent, UIComponent* detach) noexcept;

	UIComponent* m_component;
};

//public 함수들끼리 조합이 가능하면 여기서 처리한다.
bool ChangeSizeX(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept;
bool ChangeSizeY(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept;

bool IsToolMode(UIComponent* component) noexcept;

template<typename PointerType>
inline pair<unique_ptr<PointerType>, PointerType*> GetPtrs(unique_ptr<PointerType> uniquePtr)
{
	return make_pair(move(uniquePtr), uniquePtr.get());
}

template<typename T, typename... Args>
unique_ptr<T> CreateIfSetup(unique_ptr<T> obj, Args&&... args)
{
	return obj && obj->Setup(forward<Args>(args)...) ? move(obj) : nullptr;
}