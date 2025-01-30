#pragma once
#include "UIComponent.h"

class UIComponent;

class UIComponentEx
{
public:
	static unique_ptr<UIComponent> Clone(const UIComponent* component);

	static Rectangle GetRectangle(const UIComponent* component) noexcept;
	static const XMUINT2& GetSize(const UIComponent* component) noexcept;
	static UIComponent* GetComponent(UIComponent* component, const string& name) noexcept;
private:

};

template<typename T>
T GetCastComponent(UIComponent* component, const string& name) noexcept
{
	UIComponent* find = UIComponentEx::GetComponent(component, name);
	return ComponentCast<T>(find);
}

//인클루드 하면 UIComponentEx::함수이름 이렇게 호출해야 하는데 BIND_STATIC_FUNC해 주면 그냥 함수 이름만 호출해도 된다.
#define BIND_STATIC_FUNC(func) inline auto func = bind_front(UIComponentEx::func);

BIND_STATIC_FUNC(Clone);
BIND_STATIC_FUNC(GetRectangle);
BIND_STATIC_FUNC(GetSize);
BIND_STATIC_FUNC(GetComponent);