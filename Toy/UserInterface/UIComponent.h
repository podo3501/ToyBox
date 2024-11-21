#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UILayout;
class TestClass;
class JsonOperation;
class TransformComponent;

class UIComponent
{
protected:
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능

public:
	virtual ~UIComponent();
	UIComponent(const string& name, const Rectangle& rect);
	UIComponent(const UIComponent& other);
	UIComponent(UIComponent&& o) noexcept;
	UIComponent& operator=(const UIComponent& other);
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual string GetType() const;

	virtual unique_ptr<UIComponent> Clone();
	virtual void Render(IRender* render);
	virtual bool LoadResources(ILoadData* load);
	virtual bool SetDatas(IGetValue*);
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

	virtual void SerializeIO(JsonOperation* operation);
	
private:
	TransformComponent* FindTransformComponent(const string& name) const noexcept;

	string m_name{};
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<TransformComponent>> m_components;
};