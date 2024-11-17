#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UILayout;
class TestClass;
class TransformComponent;

class UIComponent
{
public:
	UIComponent();
	virtual ~UIComponent();
	UIComponent(const UIComponent& other);
	UIComponent(UIComponent&& o) noexcept;
	UIComponent& operator=(const UIComponent& other);
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual string GetType() const;

	virtual unique_ptr<UIComponent> Clone();
	virtual void Render(IRender* render);
	virtual bool LoadResources(ILoadData* load);
	virtual bool SetDatas(IGetValue*) { return true; }
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* mouseTracker) noexcept;

	void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);
	bool ChangeArea(const Rectangle& area) noexcept;
	bool IsPicking(const Vector2& pos)  const noexcept;

	UIComponent* GetComponent(const string& name) const noexcept;
	UIComponent* GetSelected() const noexcept;
	const Rectangle& GetArea() const noexcept;
	const string& GetName() const noexcept;
	UILayout* GetLayout() const noexcept;

	void SetSelected(const string& name, bool selected) noexcept;
	void SetChildPosition(const string& name, const Vector2& pos) noexcept;
	void SetName(const string& name) noexcept;
	void SetLayout(const UILayout& layout) noexcept;

	friend void to_json(nlohmann::ordered_json& j, const UIComponent& data);
	friend void from_json(const nlohmann::json& j, UIComponent& data);
	
private:
	TransformComponent* FindTransformComponent(const string& name) const noexcept;

	string m_name{};
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<TransformComponent>> m_components;
};