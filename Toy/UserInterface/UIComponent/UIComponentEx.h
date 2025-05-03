#pragma once

struct IRenderer;
class UIComponent;
class TextureResourceBinder;

//UIComponentŬ�������� public�� ���� �͵鸸 ���⿡�� �����Ѵ�. 
// �Լ��� UIComponent�� private�Լ��� �� ��� �Ѵٸ� �� Ŭ�������� �����Ѵ�.
// protected �ȿ� �Լ��� ����å���� ���� ���⿡ �ű��� �ʴ´�. ĸ��ȭ�� �����°� �� �����̱� �����̴�.
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
	vector<UIComponent*> GetRenderComponents(const XMINT2& pos) noexcept;
	XMUINT2 GetChildrenBoundsSize() const noexcept;
	bool IsPositionUpdated() const noexcept;

private:
	unique_ptr<UIComponent> DetachChild(UIComponent* parent, UIComponent* detach) noexcept;

	UIComponent* m_component;
};

//public �Լ��鳢�� ������ �����ϸ� ���⼭ ó���Ѵ�.
bool ChangeSizeX(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept;
bool ChangeSizeY(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept;

template<typename PointerType>
inline pair<unique_ptr<PointerType>, PointerType*> GetPtrs(unique_ptr<PointerType> uniquePtr)
{
	return make_pair(move(uniquePtr), uniquePtr.get());
}

//Setup�Լ��� �־�� �ϸ� ���ڰ� ��ġ �ؾ� �Ѵ�. Args�� �Ϲ��� ĳ������ ���� �ʴ´�.
template<typename T, typename... Args>
unique_ptr<T> CreateComponent(Args&&... args)
{
	auto obj = make_unique<T>();
	return obj && obj->Setup(forward<Args>(args)...) ? move(obj) : nullptr;
}