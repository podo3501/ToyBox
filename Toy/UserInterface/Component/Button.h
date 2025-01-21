#pragma once
#include "../UIComponent.h"

enum class ButtonState;
class InputManager;

class Button : public UIComponent
{
public:
	~Button();
	Button();

	static ComponentID GetTypeStatic() { return ComponentID::Button; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& other) const noexcept override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool LoadResources(ILoadData* load);
	bool SetImage(const UILayout& layout,
		unique_ptr<UIComponent>&& normal,
		unique_ptr<UIComponent>&& hover,
		unique_ptr<UIComponent>&& pressed) noexcept;

protected:
	Button(const Button& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementInput(const InputManager& inputManager) noexcept override;
	
private:
	void AddComponentAndEnable(ButtonState btnState, unique_ptr<UIComponent>&& component, bool enable) noexcept;
	void EnableButtonImage(ButtonState btnState);
	void ReloadDatas() noexcept;

	map<ButtonState, UIComponent*> m_images;
	ButtonState m_state;
};

unique_ptr<Button> CreateButton(const UILayout& layout, vector<unique_ptr<UIComponent>> imgGridList);
