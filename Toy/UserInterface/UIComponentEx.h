#pragma once

class UIComponent;

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
	vector<UIComponent*> GetComponents(const XMINT2& pos) noexcept;
	XMUINT2 GetTotalChildSize() noexcept;

private:
	unique_ptr<UIComponent> DetachChild(UIComponent* parent, UIComponent* detach) noexcept;
	Rectangle GetTotalChildSize(const UIComponent* component) noexcept;

	UIComponent* m_component;
};

//public �Լ��鳢�� ������ �����ϸ� ���⼭ ó���Ѵ�.
bool ChangeSizeX(UIComponent* component, uint32_t value) noexcept;
bool ChangeSizeY(UIComponent* component, uint32_t value) noexcept;