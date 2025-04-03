#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "UIType.h"
#include "UITransform.h"
#include "UIComponentEx.h"
#include "UIHierarchy.h"

class JsonOperation;
namespace DX { class StepTimer; }

class UIComponent : public IComponent, private UIHierarchy<UIComponent>
{
protected:
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능
	UIComponent(const string& name, const UILayout& layout) noexcept;
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept { return true; } //Binder에서 키값으로 source 얻어올때
	virtual void ImplementPositionUpdated() noexcept {}; //위치값 업데이트 할때 컴포넌트에서 추가로 설정할게 있다면
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept { return true; }
	virtual void ImplementRender(ITextureRender*) const {};
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

	//상속되어지는 함수는 구현한다.
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	vector<UIComponent*> GetChildComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;
	UIComponent* GetSiblingComponent(StateFlag::Type flag) const noexcept;
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//상속 받은 클래스도 대입생성자 기본적으로 삭제됨.

public: //이 클래스의 public 함수는 왠만하면 늘리지 않도록 하자.
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	unique_ptr<UIComponent> Clone() const;

	//IComponent virtual function(Core에서 컴포넌트를 사용할때 쓰는 함수. 로드때나 랜더링 때에는 콜백처럼 불려야 하기 때문이다. 그냥 클라이언트 값을 얻겠다고 함수를 추가하지 말자.)
	virtual void ProcessRender(ITextureRender* render) override final;

	//UIComponent virtual function(상속받은 컴포넌트들의 재정의 함수)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void SerializeIO(JsonOperation& operation);

	bool BindTextureSourceInfo(TextureResourceBinder* resBinder, ITextureController* texController) noexcept;
	bool ChangeSize(const XMUINT2& size, bool isForce = false) noexcept;
	bool UpdatePositionsManually(bool root = false) noexcept;
	bool ProcessUpdate(const DX::StepTimer& timer) noexcept;

	template<typename T>
	T GetComponent(const string& name) noexcept;
	
	inline const XMINT2& GetPosition() const noexcept { return m_transform.GetAbsolutePosition(); }
	inline const XMINT2& GetRelativePosition() const noexcept { return m_transform.GetRelativePosition(); }
	bool Rename(const string& name) noexcept;
	Rectangle GetArea() const noexcept;
	const XMUINT2& GetSize() const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); }
	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); }

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const string& GetName() const noexcept { return m_name; }
	inline bool SetStateFlag(StateFlag::Type flag, bool enabled) noexcept { return BitEnum::Set(m_stateFlag, flag, enabled); }
	inline bool HasStateFlag(StateFlag::Type flag) const noexcept { return BitEnum::Has(m_stateFlag, flag); }
	void SetChildrenStateFlag(StateFlag::Type flag, bool enabled) noexcept;
	bool RenameRegion(const string& region) noexcept;
	inline const string& GetRegion() const noexcept { return m_region; }

	inline UIComponentEx& GetUIComponentEx() noexcept
	{
		if (!m_componentEx) m_componentEx.emplace(this);
		return *m_componentEx;
	}
	
private:
	void UnlinkAndRefresh() noexcept;
	bool RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position = {}, bool active = true) noexcept;
	bool RecursivePositionUpdate(const XMINT2& position = {}) noexcept;
	UITransform& GetTransform(UIComponent* component);
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }

	string m_name;
	UILayout m_layout;
	UITransform m_transform; //이 Component가 이동되어야 하는 곳. 부모가 가져야될 데이터이나 프로그램적으로는 자기 자신이 가지는게 코드가 깔끔하다.
	StateFlag::Type m_stateFlag{ StateFlag::Default };
	string m_region; //UI에서 네임스페이스 역할을 한다. GetRegionComponent로 찾을 수 있다.

	friend class UIComponentEx;
	friend class UIHierarchy;

	optional<UIComponentEx> m_componentEx; //optional로 선언하면 포인터가 아닌데도 바로 초기화 하지 않는다.
};

inline UIComponentEx& UIEx(UIComponent* component) { return component->GetUIComponentEx(); }

template<typename Component> requires derived_from<Component, UIComponent> //UIComponent에서 상속 받은 애들만 가능
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

	using NonPointerTargetType = typename std::remove_pointer<TargetType>::type; //Target타입이 Button* 이렇게 포인터로 넘어오기 때문에 *를 떼어냄
	if (typeId == NonPointerTargetType::GetTypeStatic())
		return static_cast<TargetType>(component);

	return nullptr;
}

