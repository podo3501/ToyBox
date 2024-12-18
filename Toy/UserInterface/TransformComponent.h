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
	TransformComponent(const TransformComponent& o);
	TransformComponent(TransformComponent&& o) noexcept;
	TransformComponent& operator=(const TransformComponent& other);
	bool operator==(const TransformComponent& o) const noexcept;

	bool LoadResources(ILoadData* load);
	bool SetDatas(IGetValue* value);
	bool ProcessUpdate(const XMINT2& position, InputManager* tracker) noexcept;
	bool IsHover(const XMINT2& pos) const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	void ProcessRender(IRender* render);
	void SetPosition(const Vector2& position) noexcept;
	Vector2 GetPosition() const noexcept;
	UIComponent* GetComponent() const noexcept;
	const string& GetName() const;

	void SerializeIO(JsonOperation& operation);

public:
	unique_ptr<UIComponent> m_component;
	Vector2 m_position{};
};
