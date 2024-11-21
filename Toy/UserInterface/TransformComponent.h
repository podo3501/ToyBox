#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UIComponent;
class JsonOperation;

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(unique_ptr<UIComponent> comp, const Vector2& position);
	TransformComponent(const TransformComponent& o);
	TransformComponent(TransformComponent&& o) noexcept;
	TransformComponent& operator=(const TransformComponent& other);
	bool operator==(const TransformComponent& o) const noexcept;

	bool LoadResources(ILoadData* load);
	bool SetDatas(IGetValue* value);
	bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept;
	void SetSelected(bool selected) noexcept;
	bool IsSelected() const noexcept;
	bool IsPicking(const Vector2& pos) const noexcept;
	void Render(IRender* render);
	void SetPosition(const Vector2& position) noexcept;
	UIComponent* GetComponent() const noexcept;
	const string& GetName() const;

	void SerializeIO(JsonOperation* operation);

private:
	unique_ptr<UIComponent> m_component;
	Vector2 m_position{};
	bool m_selected{ false };
};
