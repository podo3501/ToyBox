#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "UIType.h"
#include "TransformComponent.h"

class JsonOperation;
class TransformComponent;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능
	UIComponent(const string& name, const UILayout& layout) noexcept;
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementUpdate(const XMINT2&) noexcept { return true; }
	virtual bool ImplementInput(const InputManager&) noexcept { return true; }
	virtual void ImplementRender(IRender*) const {};

	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;

	inline bool IsDirty() const noexcept { return m_isDirty; }
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//상속 받은 클래스도 대입생성자 기본적으로 삭제됨.
	UIComponent(UIComponent&& o) noexcept;
	unique_ptr<UIComponent> Clone() const;

	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	//string GetType() const;

	//IComponent virtual function(Core에서 컴포넌트를 사용할때 쓰는 함수)
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue*) override;
	virtual bool RefreshPosition() noexcept;
	virtual bool ProcessUpdate(const XMINT2& position, const InputManager& inputManager) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(상속받은 컴포넌트들의 재정의 함수)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept;
	virtual void SerializeIO(JsonOperation& operation);

	unique_ptr<UIComponent> AttachComponent(unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent() noexcept;
	
	XMINT2 GetPosition() const noexcept;
	optional<XMINT2> GetRelativePosition() const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }

	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }
	
	XMUINT2 GetTotalChildSize() const noexcept;

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const UILayout& GetLayout() const noexcept { return m_layout; }

	inline const string& GetName() const noexcept { return m_name; }
	bool Rename(const string& name) noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;

	inline void SetEnable(bool enable) noexcept { m_enable = enable; }
	inline bool IsAttachable() const noexcept;
	inline bool IsDetachable() const noexcept;
	inline void SetAttachmentState(AttachmentState state) noexcept { m_attachmentState = state; }	
	
private:
	bool IsUniqueName(const string& name, UIComponent* self) noexcept;
	void GenerateUniqueName(UIComponent* component) noexcept;
	inline bool IsInAttachmentState(AttachmentState state) const noexcept;
	bool RefreshPosition(const XMINT2& position) noexcept;
	TransformComponent* GetTransform(UIComponent* component);
	TransformComponent* FindTransformComponent(const UIComponent* component) noexcept;
	unique_ptr<UIComponent> DetachComponent(UIComponent* detachComponent) noexcept;
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	UIComponent* GetRoot() noexcept;
	void MarkDirty() noexcept;
	Rectangle GetTotalChildSize(const UIComponent* component) const noexcept;

	int m_id{ -1 };
	string m_name;
	UILayout m_layout;
	UIComponent* m_parent{ nullptr };
	bool m_enable{ true };
	bool m_isDirty{ true };
	//일단은 이동정보를 컴포넌트 갯수만큼 만드는데 나중에는 이 이동정보를 따로 분리하고 이 이동
	//정보를 인덱스로 찾는 방식으로 한다. 그러면 이동에 관련된 함수를 따로 관리 할수 있다.
	AttachmentState m_attachmentState{ AttachmentState::All };
	int m_transformID{ -1 };
	map<int, TransformComponent> m_components;
	//TransformComponent m_transform;
	
	vector<unique_ptr<UIComponent>> m_children;

	//////////////////////////////////////////////////

	void ForEachChild(std::function<void(UIComponent*)> func) noexcept;

	friend class UIComponentEx;
};

//inline
bool UIComponent::IsInAttachmentState(AttachmentState state) const noexcept {
	return m_attachmentState == state || m_attachmentState == AttachmentState::All;
}
bool UIComponent::IsAttachable() const noexcept { return IsInAttachmentState(AttachmentState::Attach); }
bool UIComponent::IsDetachable() const noexcept { return IsInAttachmentState(AttachmentState::Detach); }

#include "UIComponent.hpp"