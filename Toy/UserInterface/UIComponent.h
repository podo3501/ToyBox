#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"
#include "UIType.h"
#include "UITransform.h"
#include "UIComponentEx.h"
#include "UIHierarchy.h"

class JsonOperation;
namespace DX { class StepTimer; }

class UIComponent : public IComponent, private UIHierarchy<UIComponent>
{
protected:
	UIComponent();	//�� Ŭ������ �ܵ����� ���� �� ����. ��� ���� Ŭ�������� ���� ����
	UIComponent(const string& name, const UILayout& layout) noexcept;
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementLoadResource(ITextureLoad*) { return true; }
	virtual bool ImplementPostLoaded(ITextureController*) { return true; }
	virtual bool ImplementUpdatePosition(const DX::StepTimer&, const XMINT2&) noexcept { return true; }
	virtual bool ImplementActiveUpdate() noexcept { return true; }
	virtual void ImplementRender(ITextureRender*) const {};

	//��ӵǾ����� �Լ��� �����Ѵ�.
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool UpdatePositionsManually(const XMINT2& position = {}) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	vector<UIComponent*> GetChildComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;
	UIComponent* GetSiblingComponent(StateFlag::Type flag) const noexcept;

	inline bool IsDirty() const noexcept { return m_isDirty; }
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//��� ���� Ŭ������ ���Ի����� �⺻������ ������.

public: //�� Ŭ������ public �Լ��� �ظ��ϸ� �ø��� �ʵ��� ����.
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	unique_ptr<UIComponent> Clone() const;

	//IComponent virtual function(Core���� ������Ʈ�� ����Ҷ� ���� �Լ�. �ε嶧�� ������ ������ �ݹ�ó�� �ҷ��� �ϱ� �����̴�. �׳� Ŭ���̾�Ʈ ���� ��ڴٰ� �Լ��� �߰����� ����.)
	virtual bool LoadResources(ITextureLoad* load) override final;
	virtual bool PostLoaded(ITextureController*) override final;
	virtual void ProcessRender(ITextureRender* render) override final;

	//UIComponent virtual function(��ӹ��� ������Ʈ���� ������ �Լ�)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept;
	virtual void SerializeIO(JsonOperation& operation);

	bool ProcessUpdate(const DX::StepTimer& timer) noexcept;

	template<typename T>
	T GetComponent(const string& name) noexcept;
	
	XMINT2 GetPosition() const noexcept;
	bool Rename(const string& name) noexcept;
	Rectangle GetRectangle() const noexcept;
	const XMUINT2& GetSize() const noexcept;
	optional<XMINT2> GetRelativePosition() const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }
	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const string& GetName() const noexcept { return m_name; }
	inline void SetStateFlag(StateFlag::Type flag, bool enabled) noexcept { m_stateFlag = enabled ? (m_stateFlag | flag) : (m_stateFlag & ~flag); }
	inline bool HasStateFlag(StateFlag::Type flag) const noexcept { return (m_stateFlag & flag) != 0; }
	bool RenameRegion(const string& region) noexcept;
	inline const string& GetRegion() const noexcept { return m_region; }

	inline UIComponentEx& GetUIComponentEx() noexcept
	{
		if (!m_componentEx) m_componentEx.emplace(this);
		return *m_componentEx;
	}

private:
	void UnlinkAndRefresh() noexcept;
	const XMINT2& GetUpdatedPosition(UIComponent* component, const XMINT2& parentPos) noexcept;
	bool RecursiveUpdatePositionsManually(const DX::StepTimer& timer, const XMINT2& position) noexcept;
	bool RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position = {}, bool active = true) noexcept;
	UITransform& GetTransform(UIComponent* component);
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	void MarkDirty() noexcept;

	string m_name;
	UILayout m_layout;
	UITransform m_transform; //�� Component�� �̵��Ǿ�� �ϴ� ��. �θ� �����ߵ� �������̳� ���α׷������δ� �ڱ� �ڽ��� �����°� �ڵ尡 ����ϴ�.

	bool m_isDirty{ true };
	string m_region; //UI���� ���ӽ����̽� ������ �Ѵ�. GetRegionComponent�� ã�� �� �ִ�.
	int m_stateFlag{ StateFlag::Default };

	friend class UIComponentEx;
	friend class UIHierarchy;

	optional<UIComponentEx> m_componentEx; //optional�� �����ϸ� �����Ͱ� �ƴѵ��� �ٷ� �ʱ�ȭ ���� �ʴ´�.
};

inline UIComponentEx& UIEx(UIComponent* component) { return component->GetUIComponentEx(); }

template<typename Component> requires derived_from<Component, UIComponent> //UIComponent���� ��� ���� �ֵ鸸 ����
inline UIComponentEx& UIEx(const unique_ptr<Component>& component)
{ 
	return component->GetUIComponentEx(); 
}

//////////////////////////////////////////////////////////////////////////

template <typename TargetType>
TargetType ComponentCast(UIComponent* component)
{
	if (!component) return nullptr;
	ComponentID typeId = component->GetTypeID();
	if (typeId == ComponentID::Unknown) return nullptr;

	using NonPointerTargetType = typename std::remove_pointer<TargetType>::type; //TargetŸ���� Button* �̷��� �����ͷ� �Ѿ���� ������ *�� ���
	if (typeId == NonPointerTargetType::GetTypeStatic())
		return static_cast<TargetType>(component);

	return nullptr;
}

