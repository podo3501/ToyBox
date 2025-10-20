#pragma once

struct IRenderer;
class UIComponent;
class TextureResourceBinder;
class UINameGenerator;
class UIModule;

//UIComponentŬ�������� public�� ���� �͵鸸 ���⿡�� �����Ѵ�. 
// �Լ��� UIComponent�� private�Լ��� �� ��� �Ѵٸ� �� Ŭ�������� �����Ѵ�.
// protected �ȿ� �Լ��� ����å���� ���� ���⿡ �ű��� �ʴ´�. ĸ��ȭ�� �����°� �� �����̱� �����̴�.
class UIComponentEx
{
public:
	UIComponentEx(UIComponent* component) noexcept;

	unique_ptr<UIComponent> AttachComponent(unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
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
	bool IsPositionUpdated() const noexcept;

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

//public �Լ��鳢�� ������ �����ϸ� ���⼭ ó���Ѵ�.
bool ChangeSizeX(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept;
bool ChangeSizeY(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept;

//Setup�Լ��� �־�� �ϸ� ���ڰ� ��ġ �ؾ� �Ѵ�. Args�� �Ϲ��� ĳ������ ���� �ʴ´�.
template<typename T, typename... Args>
unique_ptr<T> CreateComponent(Args&&... args)
{
	auto obj = make_unique<T>();
	return obj && obj->Setup(forward<Args>(args)...) ? move(obj) : nullptr;
}