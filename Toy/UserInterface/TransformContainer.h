#pragma once
#include "UILayout.h"

struct ILoadData;
struct IGetValue;
class InputManager;
class UIComponent;
class TransformComponent;
class JsonOperation;

class TransformContainer
{
public:
	TransformContainer();
	TransformContainer(UIComponent* component, const string& name, const UILayout& layout);
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
	vector<TransformComponent>& GetTransformComponents() noexcept { return m_components; }
	void SetParents(TransformContainer* parent) noexcept;
	void AttachChildComponent(
		unique_ptr<UIComponent> component, const XMUINT2& size, const XMINT2& relativePos) noexcept;
	unique_ptr<UIComponent> DetachChildComponent(UIComponent* detach) noexcept;
	void SerializeIO(JsonOperation& operation);
	inline const string& GetName() const noexcept { return m_name; }

	void ForEachComponent(const function<void(TransformComponent&)>& action) noexcept;
	bool AllOfComponent(const function<bool(TransformComponent&)>& action) noexcept;
	UIComponent* FindIfComponent(const function<UIComponent* (TransformComponent&)>& action) noexcept;
	TransformComponent* FindIfTransformComponent(
		const function<bool(const TransformComponent&)>& action) noexcept;

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
	inline bool IsArea(const XMINT2& pos) const noexcept { return m_layout.IsArea(pos); }
	inline void ApplySize(const XMUINT2& size) noexcept { m_layout.Set(size); MarkDirty(); }
	inline void ChangeOrigin(Origin origin) noexcept { m_layout.Set(origin); MarkDirty(); }
	inline void SetSize(const XMUINT2& size) { m_layout.Set(size); MarkDirty(); }
	inline void SetLayout(const UILayout& layout) noexcept { m_layout = layout; }
	inline const UILayout& GetLayout() const noexcept { return m_layout; }
	vector<UIComponent*> GetComponents() const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	XMINT2 GetPositionByLayout(const XMINT2& position) const noexcept;
	XMINT2 GetPosition() const noexcept;
	void GenerateUniqueName(TransformContainer* addable) noexcept;
	XMUINT2 GetTotalChildSize() const noexcept;
	void ChangeSize(const XMUINT2& size) noexcept;
	bool ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept;

private:
	bool EqualComponent(const TransformContainer* lhs, const TransformContainer* rhs) const noexcept;
	bool IsUniqueName(const string& name, TransformContainer* self) noexcept;
	TransformComponent* FindIfTransformComponent(const string& name) noexcept;
	TransformComponent* FindIfTransformComponent(const UIComponent* component) noexcept;
	Rectangle GetTotalChildSize(const TransformContainer* transformContainer) const noexcept;

public:
	UIComponent* m_component{ nullptr };
	string m_name;
	UILayout m_layout;
	bool m_isDirty{ true };
	TransformContainer* m_parent{ nullptr };
	vector<TransformComponent> m_components;
};
