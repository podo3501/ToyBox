#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"

class JsonOperation;
class TransformComponent;
enum class Origin;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//�� Ŭ������ �ܵ����� ���� �� ����. ��� ���� Ŭ�������� ���� ����
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual void Render(IRender*) {};
	virtual bool Update(const XMINT2&, InputManager*) noexcept { return true; }
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;

public:
	virtual ~UIComponent();
	UIComponent(const string& name, const XMUINT2& size);
	UIComponent& operator=(const UIComponent&) = delete;	//��� ���� Ŭ������ ���Ի����� �⺻������ ������.
	UIComponent(UIComponent&& o) noexcept;
	unique_ptr<UIComponent> Clone() const;
	string GetType() const;

	//IComponent virtual function(Core���� ������Ʈ�� ����Ҷ� ���� �Լ�)
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue*) override;
	virtual bool ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(��ӹ��� ������Ʈ���� ������ �Լ�)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept { m_layout.Set(size); }
	virtual void SerializeIO(JsonOperation& operation);

	void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);
	
	XMINT2 GetPosition() const noexcept;
	bool ChangePosition(int index, const Vector2& pos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); }

	void SetChildPosition(const string& name, const Vector2& pos) noexcept;

	inline void SetParent(UIComponent* component) { m_parent = component; }

	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void SetEnable(bool enable) { m_enable = enable; }

	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); }
	inline const XMUINT2& GetSize() const noexcept { return m_layout.GetSize(); }

	inline void SetName(const string& name) noexcept { m_name = name; }
	inline const string& GetName() const noexcept { return m_name; }

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline UILayout GetLayout() const noexcept { return m_layout; }

	inline void SetSelected(bool selected) noexcept { m_selected = selected; }
	inline bool GetSelected() const noexcept { return m_selected; }

	UIComponent* GetComponent(const string& name) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	template<typename T>
	bool GetComponent(const string& name, T** outComponent) const noexcept;
	
private:
	const TransformComponent* FindTransformComponent(const string& name) const noexcept;
	XMINT2 GetPositionRecursive(const UIComponent* component) const noexcept;
	XMINT2 GetComponentPosition(const UIComponent* component) const noexcept;

	string m_name{};
	UILayout m_layout;
	UIComponent* m_parent{ nullptr };
	vector<TransformComponent> m_components;
	bool m_enable{ true };
	bool m_selected{ false };	//���� ����
};

#include "UIComponent.hpp"