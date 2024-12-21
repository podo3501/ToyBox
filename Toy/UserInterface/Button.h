#pragma once
#include "UIComponent.h"

enum class ButtonState;
class InputManager;

class Button : public UIComponent
{
protected:
	Button(const Button& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~Button();
	Button();

	virtual bool operator==(const UIComponent& other) const noexcept override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool LoadResources(ILoadData* load);
	bool SetImage(const string& name, const UILayout& layout,
		unique_ptr<UIComponent>&& normal,
		unique_ptr<UIComponent>&& hover,
		unique_ptr<UIComponent>&& pressed);
	bool Update(const XMINT2& position, InputManager* inputManager) noexcept;

private:
	bool UpdateButton(const XMINT2&, InputManager* inputManager) noexcept;
	void AddComponentAndEnable(ButtonState btnState, unique_ptr<UIComponent>&& component, bool enable);
	void EnableButtonImage(ButtonState btnState);
	void ReloadDatas() noexcept;

	map<ButtonState, UIComponent*> m_images;
	ButtonState m_state;
};
