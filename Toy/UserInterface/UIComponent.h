#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "UIType.h"

class JsonOperation;
class TransformComponent;
class TransformContainer;

class UIComponent;
class TransformContainer
{
public:
	TransformContainer();
	TransformContainer(UIComponent* component, const string& name);
	TransformContainer(TransformContainer&& o) noexcept;
	TransformContainer& operator=(const TransformContainer& other);
	TransformContainer& operator=(TransformContainer&& o) noexcept;
	bool operator==(const TransformContainer& o) const noexcept;

	bool LoadResources(ILoadData* load);
	bool SetDatas(IGetValue* value);
	bool Update(const InputManager& inputManager, const UILayout& layout, const XMINT2& position);
	void PushChildComponents(UIComponent* component, queue<UIComponent*>& outQueue);
	void AdjustChildPosition(const XMUINT2& size);
	bool ChangeChildPosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	vector<TransformComponent>& GetComponents() noexcept { return m_components; }
	void SetParents(TransformContainer* parent) noexcept;
	void AttachChildComponent(
		unique_ptr<UIComponent> component, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	unique_ptr<UIComponent> DetachChildComponent(UIComponent* detach) noexcept;
	void SerializeIO(JsonOperation& operation);
	Rectangle GetTotalChildSize(const UIComponent* component) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;

	void ForEachComponent(const function<void(TransformComponent&)>& action) noexcept;
	bool AllOfComponent(const function<bool(TransformComponent&)>& action) noexcept;
	UIComponent* FindIfComponent(const function<UIComponent* (TransformComponent&)>& action) noexcept;
	TransformComponent* FindIfTransformComponent(
		const function<bool(const TransformComponent&)>& action) noexcept;

	inline const string& GetName() const noexcept { return m_name; }
	TransformContainer* GetComponent(const string& name) noexcept;
	inline void SetParent(TransformContainer* transformContainer) noexcept { m_parent = transformContainer; }
	inline TransformContainer* GetParent() const noexcept { return m_parent; }
	inline UIComponent* GetComponent() const noexcept { return m_component; }
	inline void SetComponent(UIComponent* component) noexcept { m_component = component; }
	bool Rename(const string& name) noexcept;
	optional<XMINT2> GetRelativePosition() const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	void MarkDirty() noexcept;
	bool GetDirty() const noexcept { return m_isDirty; }
	void SetDirty(bool dirty) noexcept { m_isDirty = dirty; }
	XMINT2 GetParentPosition() const noexcept;
	TransformContainer* GetRoot() noexcept;
	bool IsUniqueName(const string& name, UIComponent* self) noexcept;

private:
	bool EqualComponent(const TransformContainer* lhs, const TransformContainer* rhs) const noexcept;
	bool IsUniqueName(const string& name, TransformContainer* self) noexcept;
	TransformComponent* FindIfTransformComponent(const string& name) noexcept;
	TransformComponent* FindIfTransformComponent(const UIComponent* component) noexcept;

public:
	string m_name;
	bool m_isDirty{ true };
	UIComponent* m_component{ nullptr };
	TransformContainer* m_parent{ nullptr };
	vector<TransformComponent> m_components;
};

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

	inline bool IsDirty() const noexcept { return m_transformContainer.GetDirty(); }
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); m_transformContainer.MarkDirty(); }
	
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
	//optional<XMINT2> GetRelativePosition() const noexcept;
	//bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); m_transformContainer.MarkDirty(); }

	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); m_transformContainer.MarkDirty(); }
	
	XMUINT2 GetTotalChildSize() const noexcept;

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const UILayout& GetLayout() const noexcept { return m_layout; }

	/*inline const string& GetName() const noexcept { return m_name; }*/
	//bool Rename(const string& name) noexcept;
	//UIComponent* GetComponent(const string& name) noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	template<typename T>//hpp에 있음.
	void GetComponent(const string& name, T** outComponent) noexcept;

	inline void SetEnable(bool enable) noexcept { m_enable = enable; }
	inline bool IsAttachable() const noexcept;
	inline bool IsDetachable() const noexcept;
	inline void SetAttachmentState(AttachmentState state) noexcept { m_attachmentState = state; }	
	inline const TransformContainer& GetTransformContainer() const noexcept { return m_transformContainer; }
	inline TransformContainer& GetTransformContainer() noexcept { return m_transformContainer; }
	
private:
	//bool IsUniqueName(const string& name, UIComponent* self) noexcept;
	void GenerateUniqueName(UIComponent* component) noexcept;
	inline bool IsInAttachmentState(AttachmentState state) const noexcept;
	bool RefreshPosition(const XMINT2& position) noexcept;
	TransformComponent* FindIfTransformComponent(const string& name) noexcept;
	TransformComponent* FindIfTransformComponent(const UIComponent* component) noexcept;
	unique_ptr<UIComponent> DetachComponent(UIComponent* detachComponent) noexcept;
	//UIComponent* GetRoot() noexcept;
	//void MarkDirty() noexcept;

	friend class TransformContainer;

	//string m_name;
	UILayout m_layout;
	//UIComponent* m_parent{ nullptr };
	bool m_enable{ true };
	///*bool m_isDirty{ true };*/
	AttachmentState m_attachmentState{ AttachmentState::All };
	//vector<TransformComponent> m_componentss;
	TransformContainer m_transformContainer;
};

//inline
bool UIComponent::IsInAttachmentState(AttachmentState state) const noexcept {
	return m_attachmentState == state || m_attachmentState == AttachmentState::All;
}
bool UIComponent::IsAttachable() const noexcept { return IsInAttachmentState(AttachmentState::Attach); }
bool UIComponent::IsDetachable() const noexcept { return IsInAttachmentState(AttachmentState::Detach); }

#include "UIComponent.hpp"