#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;
enum class ButtonState;

class Button : public UIComponent
{
public:
	virtual ~Button();
	Button();
	Button(const Button& other);
	bool operator==(const UIComponent& o) const noexcept;

	virtual unique_ptr<UIComponent> Clone() override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, MouseTracker* tracker) noexcept override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(const string& name,
		const UILayout& layout,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed);
	void ChangeOrigin(Origin&& origin) noexcept;
	bool ChangeArea(const Rectangle& area) noexcept;

	void SerializeIO(JsonOperation& operation);

private:
	bool SetDestination(const Rectangle& area) noexcept;
	void InsertImage(ButtonState btnButtonState, unique_ptr<ImagePartSet>&& imgPartSet);

	ButtonState m_state;
	map<ButtonState, unique_ptr<ImagePartSet>> m_image;
};
