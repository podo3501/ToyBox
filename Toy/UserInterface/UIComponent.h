#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UILayout;
class JsonOperation;
class TestClass;

class UIComponent
{
public:
	UIComponent();
	virtual ~UIComponent();
	UIComponent(const UIComponent& other);
	bool IsEqual(const UIComponent* other) const noexcept;

	virtual unique_ptr<UIComponent> Clone() = 0;
	virtual bool ReadProperty(const nlohmann::json&) { return true; }	//Property는 Component의 구현부이기 때문에 상속받지 않기 때문에 각각의 클래스에서 재정의가 필요하다.
	virtual void Render(IRender* render);
	virtual bool SetResources(const wstring& filename);
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

	void FileIO(JsonOperation* operation) noexcept;
	//static unique_ptr<UIComponent> CreateComponent(JsonOperation* operation);

	friend void to_json(nlohmann::json& j, const UIComponent& comp);
	friend void from_json(const nlohmann::json& j, UIComponent& comp);

protected:
	void ToJson(nlohmann::ordered_json& outJson) const noexcept;
	void FromJson(const nlohmann::json& j) noexcept;
	virtual void Process(JsonOperation*) noexcept {};
	
private:
	class Property;
	Property* FindProperty(const string& name) const noexcept;

	string m_name{};
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<Property>> m_properties;
	vector<unique_ptr<TestClass>> m_testClass;
};