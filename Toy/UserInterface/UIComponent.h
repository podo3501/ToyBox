#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "UIType.h"

class JsonOperation;
class TransformComponent;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//�� Ŭ������ �ܵ����� ���� �� ����. ��� ���� Ŭ�������� ���� ����
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementUpdate(const XMINT2&) noexcept { return true; }
	virtual bool ImplementInput(const InputManager&) noexcept { return true; }
	virtual void ImplementRender(IRender*) const {};

	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;

	inline void SetName(const string& name) noexcept { m_name = name; }
	inline bool IsDirty() const noexcept { return m_isDirty; }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }

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
	virtual bool RefreshPosition() noexcept;
	virtual bool ProcessUpdate(const XMINT2& position, const InputManager& inputManager) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(��ӹ��� ������Ʈ���� ������ �Լ�)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept;
	virtual void SerializeIO(JsonOperation& operation);

	bool AttachComponent(unique_ptr<UIComponent>&& component, const XMINT2& relativePos) noexcept;
	optional<unique_ptr<UIComponent>> DetachComponent() noexcept;
	
	XMINT2 GetPosition() const noexcept;
	bool GetRelativePosition(XMINT2& outRelativePos) const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }

	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }

	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }
	inline const XMUINT2& GetSize() const noexcept { return m_layout.GetSize(); }
	XMUINT2 GetTotalChildSize() const noexcept;

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const UILayout& GetLayout() const noexcept { return m_layout; }

	inline const string& GetName() const noexcept { return m_name; }
	bool Rename(const string& name) noexcept;
	UIComponent* GetComponent(const string& name) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	template<typename T>
	bool GetComponent(const string& name, T** outComponent) const noexcept;

	inline void SetEnable(bool enable) { m_enable = enable; }
	bool IsAttachable() const noexcept;
	inline void SetAttachmentState(AttachmentState state) noexcept { m_attachmentState = state; }	
	
private:
	bool RefreshPosition(const XMINT2& position) noexcept;
	TransformComponent* FindTransformComponent(const string& name) noexcept;
	TransformComponent* FindTransformComponent(const UIComponent* component) noexcept;
	optional<unique_ptr<UIComponent>> DetachComponent(UIComponent* detachComponent) noexcept;
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	UIComponent* GetRoot() noexcept;
	void MarkDirty() noexcept;
	Rectangle GetTotalChildSize(const UIComponent* component) const noexcept;

	string m_name;
	UILayout m_layout;
	UIComponent* m_parent{ nullptr };
	vector<TransformComponent> m_components;
	bool m_enable{ true };
	bool m_isDirty{ true };
	AttachmentState m_attachmentState{ AttachmentState::All };
};

#include "UIComponent.hpp"