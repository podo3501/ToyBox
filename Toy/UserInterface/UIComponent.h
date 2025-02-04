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
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능
	UIComponent(const string& name, const UILayout& layout) noexcept;
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementUpdatePosition(const XMINT2&) noexcept { return true; }
	virtual bool ImplementActiveUpdate(const XMINT2&) noexcept { return true; }
	virtual void ImplementRender(IRender*) const {};

	//상속되어지는 함수는 구현한다.
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	vector<UIComponent*> GetChildComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;

	inline bool IsDirty() const noexcept { return m_isDirty; }
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//상속 받은 클래스도 대입생성자 기본적으로 삭제됨.
	UIComponent(UIComponent&& o) noexcept;

public: //이 클래스의 public 함수는 왠만하면 늘리지 않도록 하자.
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	unique_ptr<UIComponent> Clone() const;

	//IComponent virtual function(Core에서 컴포넌트를 사용할때 쓰는 함수)
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue*) override;
	virtual bool ProcessUpdate(const XMINT2& position, bool activeUpdate) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(상속받은 컴포넌트들의 재정의 함수)
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
	UITransform m_transform; //이 Component가 이동되어야 하는 곳. 부모가 가져야될 데이터이나 프로그램적으로는 자기 자신이 가지는게 코드가 깔끔하다.

	bool m_enable{ true };
	bool m_isDirty{ true };
	bool m_region{ false }; //이 노드 이후는 앞 노드들과 이름이 중복되어도 상관없다. UI에서 네임스페이스 역할을 해 준다.
	AttachmentState m_attachmentState{ AttachmentState::All };
	
	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<UIComponent>> m_children;

	friend class UIComponentEx;
	optional<UIComponentEx> m_componentEx; //optional로 선언하면 포인터가 아닌데도 바로 초기화 하지 않는다.
};

inline UIComponentEx& UIEx(UIComponent* component) { return component->GetUIComponentEx(); }

template<typename Component> requires derived_from<Component, UIComponent> //UIComponent에서 상속 받은 애들만 가능
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

