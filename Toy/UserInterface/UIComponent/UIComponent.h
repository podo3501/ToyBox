#pragma once

#include "IComponent.h"
#include "UILayout.h"
#include "UITransform.h"
#include "UIComponentEx.h"
#include "UIHierarchy.h"

class SerializerIO;
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
	virtual bool ImplementResizeAndAdjustPos(const XMUINT2& size) noexcept; //사이즈 바꾸면 child의 위치를 바꿔야 한다.
	virtual bool ImplementChangeSize(const XMUINT2&, bool) noexcept { return true; } //사이즈 바꿀때
	virtual bool EnterToolMode() noexcept { return true;	}
	virtual bool ExitToolMode() noexcept { return true; }

	//상속되어지는 함수는 구현한다.
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool ChangePosition(size_t index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	vector<UIComponent*> GetChildComponents() const noexcept;
	UIComponent* GetChildComponent(size_t index) const noexcept;
	UIComponent* GetSiblingComponent(StateFlag::Type flag) const noexcept;
	inline void SetRenderTraversal(RenderTraversal traversal) noexcept { m_renderTraversal = traversal; }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); } //?!? SetSize와 같은 역할이다.
	inline bool GetToolMode() const noexcept { return m_toolMode; }
	
public:
	virtual ~UIComponent();
	UIComponent& operator=(const UIComponent&) = delete;	//상속 받은 클래스도 대입생성자 기본적으로 삭제됨.

public: //이 클래스의 public 함수는 왠만하면 늘리지 않도록 하자.
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept = 0;
	unique_ptr<UIComponent> Clone() const;

	//IComponent virtual function(Core에서 컴포넌트를 사용할때 쓰는 함수. 로드때나 랜더링 때에는 콜백처럼 불려야 하기 때문이다. 그냥 클라이언트 값을 얻겠다고 함수를 추가하지 말자.)
	virtual void ProcessRender(ITextureRender* render) override final;
	//마우스 관련 event
	virtual void OnNormal() noexcept {}
	virtual void OnHover() noexcept {}
	virtual void OnMove(const XMINT2& pos) noexcept { pos; }
	virtual InputResult OnPress(const XMINT2& pos) noexcept { pos; return InputResult::None; }
	virtual void OnHold(const XMINT2& pos, bool inside) noexcept { pos; inside; }
	virtual void OnRelease(bool inside) noexcept { inside; }
	virtual bool OnWheel(int wheelValue) noexcept { wheelValue; return false; }

	//UIComponent virtual function(상속받은 컴포넌트들의 재정의 함수)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ProcessIO(SerializerIO& serializer);

	bool BindTextureSourceInfo(TextureResourceBinder* resBinder, ITextureController* texController) noexcept;
	bool ChangeSize(const XMUINT2& size, bool isForce = false) noexcept; //isForce는 크기가 변함이 없더라도 끝까지 실행시킨다.
	inline bool ChangeSize(uint32_t x, uint32_t y, bool isForce = false) noexcept { return ChangeSize({ x, y }, isForce); }
	bool UpdatePositionsManually(bool root = false) noexcept;
	bool ProcessUpdate(const DX::StepTimer& timer) noexcept;
	
	inline const XMINT2& GetLeftTop() const noexcept { return m_transform.GetAbsolutePosition(); }
	inline const XMINT2& GetRelativePosition() const noexcept { return m_transform.GetRelativePosition(); }
	Rectangle GetArea() const noexcept;
	const XMUINT2& GetSize() const noexcept;
	bool ChangeRelativePosition(const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); }
	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); }
	bool EnableToolMode(bool enable) noexcept;

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const string& GetName() const noexcept { return m_name; }
	inline bool IsVisible() const noexcept { return HasStateFlag(StateFlag::Render); }
	inline void SetVisible(bool visible) noexcept { SetStateFlag(StateFlag::Render, visible); }
	inline bool SetStateFlag(StateFlag::Type flag, bool enabled) noexcept 
	{ 
		return BitEnum::Set(m_stateFlag, flag, enabled); 
	}
	inline bool HasStateFlag(StateFlag::Type flag) const noexcept { return BitEnum::Has(m_stateFlag, flag); }
	void SetChildrenStateFlag(StateFlag::Type flag, bool enabled) noexcept;
	inline const string& GetRegion() const noexcept { return m_region; } //현재 이 컴포넌트의 region값
	string GetMyRegion() const noexcept; //자기가 속해있는 region값

	inline UIComponentEx& GetUIComponentEx() noexcept
	{
		if (!m_componentEx) m_componentEx.emplace(this);
		return *m_componentEx;
	}
	
private:
	void UnlinkAndRefresh() noexcept;
	bool RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position = {}) noexcept;
	bool RecursivePositionUpdate(const XMINT2& position = {}) noexcept;
	UITransform& GetTransform(UIComponent* component);
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	inline RenderTraversal GetRenderSearchType() const noexcept { return m_renderTraversal; }

	string m_name;
	UILayout m_layout;
	UITransform m_transform; //이 Component가 이동되어야 하는 곳. 부모가 가져야될 데이터이나 프로그램적으로는 자기 자신이 가지는게 코드가 깔끔하다.
	StateFlag::Type m_stateFlag{ StateFlag::Default };
	string m_region; //UI에서 네임스페이스 역할을 한다. FindRegionComponent로 찾을 수 있다.
	RenderTraversal m_renderTraversal{ RenderTraversal::Inherited }; //이건 mode이기 때문에 flag와 성격이 맞지 않아서 따로 만듦. 지금은 2개뿐이라 flag에 넣어도 되긴한데, 추후 확장성을 고려해서 일단 이렇게 놔두기로 하자.

	bool m_toolMode{ false };
	optional<UIComponentEx> m_componentEx; //optional로 선언하면 포인터가 아닌데도 바로 초기화 하지 않는다.

	friend class UIComponentEx;
	friend class UIHierarchy;
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

//파생 vs 부모, 부모 vs 파생, 파생 vs 파생 비교 할 수 있게끔 헬퍼 함수.
template<typename L, typename R>
bool CompareDerived(const unique_ptr<L>& lhs, const unique_ptr<R>& rhs)
{
	static_assert(std::is_base_of_v<UIComponent, L>, "CompareDerived: L은 UIComponent 파생이어야 함");
	static_assert(std::is_base_of_v<UIComponent, R>, "CompareDerived: R은 UIComponent 파생이어야 함");

	if (lhs->GetTypeID() != rhs->GetTypeID()) return false;
	if (!lhs && !rhs) return true;
	if (!lhs || !rhs) return false;

	//부모인 UIComponent&로 업캐스팅 후 비교
	bool result = static_cast<const UIComponent&>(*lhs) ==
		static_cast<const UIComponent&>(*rhs);
	Assert(result);
	return result;
}