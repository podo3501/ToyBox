#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "TransformContainer.h"
#include "UIType.h"

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

	inline bool IsDirty() const noexcept { return m_transformContainer.GetDirty(); }
	
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

	inline void SetEnable(bool enable) noexcept { m_enable = enable; }
	inline bool IsAttachable() const noexcept;
	inline bool IsDetachable() const noexcept;
	inline void SetAttachmentState(AttachmentState state) noexcept { m_attachmentState = state; }	
	inline const TransformContainer& GetTransformContainer() const noexcept { return m_transformContainer; }
	inline TransformContainer& GetTransformContainer() noexcept { return m_transformContainer; }
	
private:
	inline bool IsInAttachmentState(AttachmentState state) const noexcept;
	bool RefreshPosition(const XMINT2& position) noexcept;
	unique_ptr<UIComponent> DetachComponent(UIComponent* detachComponent) noexcept;

public:
	void ForEachChild(std::function<void(UIComponent*)> func) noexcept;
	const string& GetName() const noexcept { return m_name; }
	UIComponent* FindRoot();

private:
	bool m_enable{ true };
	AttachmentState m_attachmentState{ AttachmentState::All };
	TransformContainer m_transformContainer;

	string m_name;
	UIComponent* m_parent{ nullptr };
	vector<UIComponent*> m_children;
};

//inline
bool UIComponent::IsInAttachmentState(AttachmentState state) const noexcept {
	return m_attachmentState == state || m_attachmentState == AttachmentState::All;
}
bool UIComponent::IsAttachable() const noexcept { return IsInAttachmentState(AttachmentState::Attach); }
bool UIComponent::IsDetachable() const noexcept { return IsInAttachmentState(AttachmentState::Detach); }

#include "UIComponent.hpp"

UIComponent* GetComponentNew(UIComponent* component, const string& targetName);