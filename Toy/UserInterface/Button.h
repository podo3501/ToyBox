#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;

class Button : public IRenderItem
{
	enum ButtonState
	{
		Normal = 0,
		Hover = 1,
		Pressed = 2,
	};

public:
	Button();
	virtual ~Button();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(
		const wstring& resPath,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed,
		const Rectangle& area, const Vector2& pos, Origin origin);
	void Update(const Vector2& resolution, const Mouse::ButtonStateTracker& tracker);
	void ChangeOrigin(Origin origin);

private:
	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, unique_ptr<ImagePartSet>> m_image;
};