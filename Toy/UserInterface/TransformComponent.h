#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UIComponent;
class JsonOperation;
class InputManager;

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(unique_ptr<UIComponent> comp, const Vector2& position);
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator=(const TransformComponent&) = delete;
	TransformComponent(TransformComponent&& o) noexcept;
	bool operator==(const TransformComponent& o) const noexcept;

	TransformComponent Clone() const;
	void SerializeIO(JsonOperation& operation);

	unique_ptr<UIComponent> m_component;
	Vector2 m_position{};
};
