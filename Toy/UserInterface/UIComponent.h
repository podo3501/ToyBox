#pragma once

#include "../Include/IComponent.h"

class UILayout;
class JsonOperation;
class TransformComponent;
enum class Origin;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//�� Ŭ������ �ܵ����� ���� �� ����. ��� ���� Ŭ�������� ���� ����

	virtual void Render(IRender*) {};
	virtual bool Update(const XMINT2&, InputManager*) noexcept { return true; }
	XMINT2 GetPositionByLayout(const XMINT2& position) noexcept;

public:
	virtual ~UIComponent();
	UIComponent(const string& name, const Rectangle& rect);
	UIComponent(const UIComponent& other);
	UIComponent(UIComponent&& o) noexcept;
	UIComponent& operator=(const UIComponent& other);
	virtual string GetType() const;

	//IComponent virtual function(Core���� ������Ʈ�� ����Ҷ� ���� �Լ�)
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue*) override;
	virtual bool ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(��ӹ��� ������Ʈ���� ������ �Լ�)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual unique_ptr<UIComponent> Clone();
	virtual bool ChangeArea(const Rectangle& area) noexcept;

	void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);
	void SetSize(const XMUINT2& size);
	XMUINT2 GetSize() const noexcept;
	
	bool ChangePosition(int index, const Vector2& pos) noexcept;
	void ChangeOrigin(const Origin& origin) noexcept;
	bool IsHover(const XMINT2& pos) const noexcept;

	UIComponent* GetComponent(const string& name) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;

	template<typename T>
	bool GetComponent(const string& name, T** outComponent) const noexcept;

	const Rectangle& GetArea() const noexcept;	
	void SetChildPosition(const string& name, const Vector2& pos) noexcept;

	inline void SetName(const string& name) noexcept { m_name = name; }
	inline const string& GetName() const noexcept { return m_name; }

	UILayout* GetLayout() const noexcept;
	void SetLayout(const UILayout& layout) noexcept;

	bool IsArea(const XMINT2& pos) const noexcept;

	void SetSelected(bool selected) noexcept;
	bool GetSelected() const noexcept;

	void SetEnable(bool enable);

	virtual void SerializeIO(JsonOperation& operation);
	
private:
	TransformComponent* FindTransformComponent(const string& name) const noexcept;

	string m_name{};
	unique_ptr<UILayout> m_layout;
	bool m_enable{ true };
	bool m_selected{ false };	//���� ����
	vector<unique_ptr<TransformComponent>> m_components;
};

template<typename T>
T ComponentCast(UIComponent* component)
{
	using NonPointerType = typename std::remove_pointer<T>::type;

	//assert�� �ϴ� ������ dynamic cast�� �� ���� �;. Ÿ���� �ٸ� ��쿡�� debug�϶��� ���⼭ �ɸ��� release�� Ÿ�Ժ�ȯ�� ������ �ൿ
	assert(component->GetType() == string(typeid(NonPointerType).name()));	
	return static_cast<T>(component);
}

template<typename T>
bool UIComponent::GetComponent(const string& name, T** outComponent) const noexcept
{
	UIComponent* component = GetComponent(name);
	*outComponent = ComponentCast<T*>(component);

	return false;
}