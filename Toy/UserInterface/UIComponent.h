#pragma once

#include "../Include/IComponent.h"
#include "UILayout.h"
#include "Property.h"

class JsonOperation;
class TransformComponent;
enum class Origin;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능
	UIComponent(const UIComponent& other);

	virtual unique_ptr<UIComponent> CreateClone() const = 0;
	virtual bool ImplementUpdate(const XMINT2&) noexcept { return true; }
	virtual bool ImplementInput(const InputManager&) noexcept { return true; }
	virtual void ImplementRender(IRender*) const {};

	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool IsDirty() const noexcept { return m_isDirty; }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }

public:
	virtual ~UIComponent();
	UIComponent(const string& name, const XMUINT2& size);
	UIComponent& operator=(const UIComponent&) = delete;	//상속 받은 클래스도 대입생성자 기본적으로 삭제됨.
	UIComponent(UIComponent&& o) noexcept;
	unique_ptr<UIComponent> Clone() const;
	string GetType() const;

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

	void AddComponent(unique_ptr<UIComponent>&& component, const XMINT2& relativePos) noexcept;
	
	XMINT2 GetPosition() const noexcept;
	bool GetRelativePosition(XMINT2& outRelativePos) const noexcept;
	bool SetRelativePosition(const XMINT2& relativePos) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }

	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void SetEnable(bool enable) { m_enable = enable; }

	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }
	inline const XMUINT2& GetSize() const noexcept { return m_layout.GetSize(); }

	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const UILayout& GetLayout() const noexcept { return m_layout; }

	UIComponent* GetComponent(const string& name) const noexcept;
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	template<typename T>
	bool GetComponent(const string& name, T** outComponent) const noexcept;

public:
	Property<string> Name{};
	
private:
	bool RefreshPosition(const XMINT2& position) noexcept;
	TransformComponent* FindTransformComponent(const string& name) noexcept;
	TransformComponent* FindTransformComponent(const UIComponent* component) noexcept;
	inline void SetParent(UIComponent* component) noexcept { m_parent = component; }
	UIComponent* GetRoot() noexcept;
	void MarkDirty() noexcept;

	UILayout m_layout;
	UIComponent* m_parent{ nullptr };
	vector<TransformComponent> m_components;
	bool m_enable{ true };
	bool m_isDirty{ true };
};

#include "UIComponent.hpp"