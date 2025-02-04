#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "UIType.h"
#include "UITransform.h"
#include "UIComponentEx.h"

class JsonOperation;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//�� Ŭ������ �ܵ����� ���� �� ����. ��� ���� Ŭ�������� ���� ����
	UIComponent(const string& name, const UILayout& layout) noexcept;
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementUpdatePosition(const XMINT2&) noexcept { return true; }
	virtual bool ImplementActiveUpdate(const XMINT2&) noexcept { return true; }
	virtual void ImplementRender(IRender*) const {};

	//��ӵǾ����� �Լ��� �����Ѵ�.
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	vector<UIComponent*> GetChildComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;

	inline bool IsDirty() const noexcept { return m_isDirty; }
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//��� ���� Ŭ������ ���Ի����� �⺻������ ������.
	UIComponent(UIComponent&& o) noexcept;

public: //�� Ŭ������ public �Լ��� �ظ��ϸ� �ø��� �ʵ��� ����.
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	unique_ptr<UIComponent> Clone() const;

	//IComponent virtual function(Core���� ������Ʈ�� ����Ҷ� ���� �Լ�)
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue*) override;
	virtual bool ProcessUpdate(const XMINT2& position, bool activeUpdate) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(��ӹ��� ������Ʈ���� ������ �Լ�)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept;
	virtual void SerializeIO(JsonOperation& operation);

	template<typename T>
	T GetComponent(const string& name) noexcept;
	
	bool Rename(const string& name) noexcept;
	Rectangle GetRectangle() const noexcept;
	const XMUINT2& GetSize() const noexcept;
	XMINT2 GetPosition() const noexcept;
	optional<XMINT2> GetRelativePosition() const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }
	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const string& GetName() const noexcept { return m_name; }
	inline void SetEnable(bool enable) noexcept { m_enable = enable; }
	inline void SetAttachmentState(AttachmentState state) noexcept { m_attachmentState = state; }
	inline void SetRegion(bool region) noexcept { m_region = region; }
	inline bool GetRegion() const noexcept { return m_region; }

	inline UIComponentEx& GetUIComponentEx() noexcept
	{
		if (!m_componentEx) m_componentEx.emplace(this);
		return *m_componentEx;
	}

private:
	inline bool IsAttachable() const noexcept;
	inline bool IsDetachable() const noexcept;
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool IsUniqueName(const string& name, UIComponent* self) noexcept;
	void GenerateUniqueName(UIComponent* component) noexcept;
	inline bool IsInAttachmentState(AttachmentState state) const noexcept;
	UITransform& GetTransform(UIComponent* component);
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	void MarkDirty() noexcept;

	void ForEachChild(function<void(UIComponent*)> func) noexcept;
	void ForEachChildBool(function<bool(UIComponent*)> func) noexcept;
	void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept;
	void ForEachChildBFS(function<void(UIComponent*)> func) noexcept;

	string m_name;
	UILayout m_layout;
	UITransform m_transform; //�� Component�� �̵��Ǿ�� �ϴ� ��. �θ� �����ߵ� �������̳� ���α׷������δ� �ڱ� �ڽ��� �����°� �ڵ尡 ����ϴ�.

	bool m_enable{ true };
	bool m_isDirty{ true };
	bool m_region{ false }; //�� ��� ���Ĵ� �� ����� �̸��� �ߺ��Ǿ �������. UI���� ���ӽ����̽� ������ �� �ش�.
	AttachmentState m_attachmentState{ AttachmentState::All };
	
	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<UIComponent>> m_children;

	friend class UIComponentEx;
	optional<UIComponentEx> m_componentEx; //optional�� �����ϸ� �����Ͱ� �ƴѵ��� �ٷ� �ʱ�ȭ ���� �ʴ´�.
};

inline UIComponentEx& UIEx(UIComponent* component) { return component->GetUIComponentEx(); }

template<typename Component> requires derived_from<Component, UIComponent> //UIComponent���� ��� ���� �ֵ鸸 ����
inline UIComponentEx& UIEx(const unique_ptr<Component>& component)
{ 
	return component->GetUIComponentEx(); 
}

//inline
bool UIComponent::IsInAttachmentState(AttachmentState state) const noexcept {
	return m_attachmentState == state || m_attachmentState == AttachmentState::All;
}
bool UIComponent::IsAttachable() const noexcept { return IsInAttachmentState(AttachmentState::Attach); }
bool UIComponent::IsDetachable() const noexcept { return IsInAttachmentState(AttachmentState::Detach); }

#include "UIComponent.hpp"

