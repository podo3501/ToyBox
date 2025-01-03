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
	virtual void Render(IRender*) const {};
	virtual bool Update(const XMINT2&, InputManager*) noexcept { return true; }
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	bool EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept;
	bool IsDirty() const noexcept { return m_isDirty; }

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
	virtual bool ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept override final;
	virtual void ProcessRender(IRender* render) override final;

	//UIComponent virtual function(상속받은 컴포넌트들의 재정의 함수)
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual void ChangeSize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	virtual void SerializeIO(JsonOperation& operation);

	void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);
	
	
	Rectangle GetRectangle() const noexcept;
	XMINT2 GetPosition() const noexcept;
	void RefreshPosition() noexcept;
	bool ChangePosition(int index, const Vector2& pos) noexcept;
	inline void ChangeOrigin(const Origin& origin) noexcept { m_layout.Set(origin); MarkDirty(); }

	void SetChildPosition(const string& name, const Vector2& pos) noexcept;

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
	const TransformComponent* FindTransformComponent(const string& name) const noexcept;
	const TransformComponent* FindTransformComponent(const UIComponent* component) const noexcept;
	XMINT2 GetComponentPosition(const UIComponent* component) const noexcept;
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