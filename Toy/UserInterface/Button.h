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
	virtual bool IsPicking(const Vector2&)  const noexcept override { return false; }
	virtual const Rectangle& GetArea() const noexcept override;
	
	void SetImage(
		const wstring& resPath,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed,
		const UILayout& layout);
	bool ChangeArea(Rectangle&& area) noexcept;
	void Update(const Vector2& resolution, const Mouse::ButtonStateTracker* tracker) noexcept;
	void ChangeOrigin(Origin&& origin) noexcept;

private:
	bool SetDestination(const Rectangle& area) noexcept;

	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, unique_ptr<ImagePartSet>> m_image;
};