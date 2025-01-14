#pragma once
#include "../UIComponent.h"

enum class ButtonState;
class InputManager;

class Button : public UIComponent
{
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

protected:
	Button(const Button& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementInput(const InputManager& inputManager) noexcept override;
	
private:
	void AddComponentAndEnable(ButtonState btnState, unique_ptr<UIComponent>&& component, bool enable);
	void EnableButtonImage(ButtonState btnState);
	void ReloadDatas() noexcept;

	map<ButtonState, UIComponent*> m_images;
	ButtonState m_state;
};
