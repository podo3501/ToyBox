#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;

class Button : public UIComponent
{
	enum ButtonState
	{
		Normal = 0,
		Hover = 1,
		Pressed = 2,
	};

public:
	virtual ~Button();
	Button();
	Button(const Button& other);

	virtual unique_ptr<UIComponent> Clone() override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(const string& name,
		const Vector2& position,
		const UILayout& layout,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed);
	void ChangeOrigin(Origin&& origin) noexcept;
	bool ChangeArea(const Rectangle& area) noexcept;
	void SetPosition(const Vector2&) noexcept;

private:
	bool SetDestination(const Rectangle& area) noexcept;

	string m_name;
	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, unique_ptr<ImagePartSet>> m_image;
	Vector2 m_position{};
};