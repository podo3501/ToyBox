#pragma once

struct ILoadData;
struct IRender;
class UIComponent;

class Property
{
public:
	Property();
	Property(unique_ptr<UIComponent> comp, const Vector2& position);
	Property(const Property& other);

	bool LoadResources(ILoadData* load);
	bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept;
	void SetSelected(bool selected) noexcept;
	bool IsSelected() const noexcept;
	bool IsPicking(const Vector2& pos) const noexcept;
	void Render(IRender* render);
	void SetPosition(const Vector2& position) noexcept;
	UIComponent* GetComponent() const noexcept;
	const string& GetName() const;

	friend void to_json(nlohmann::ordered_json& j, const Property& data);
	friend void from_json(const nlohmann::json& j, Property& data);

private:
	unique_ptr<UIComponent> m_component;
	Vector2 m_position{};
	bool m_selected{ false };
};
