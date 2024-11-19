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
	enum class State
	{
		Init = 0,
		Normal = 1,
		Hover = 2,
		Pressed = 3,
	};
	string GetStringState(State state);
	State GetStateString(const string& str);

public:
	virtual ~Button();
	Button();
	Button(const Button& other);

	virtual unique_ptr<UIComponent> Clone() override;
	//virtual bool ReadProperty(const nlohmann::json& data) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(const string& name,
		const UILayout& layout,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed);
	void ChangeOrigin(Origin&& origin) noexcept;
	bool ChangeArea(const Rectangle& area) noexcept;

	void SerializeIO(JsonOperation* operation);

private:
	bool SetDestination(const Rectangle& area) noexcept;
	void InsertImage(State btnState, unique_ptr<ImagePartSet>&& imgPartSet);

	State m_state{ State::Normal };
	map<State, unique_ptr<ImagePartSet>> m_image;
};
