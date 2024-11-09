#pragma once
#include "IRenderItem.h"

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
	virtual ~Button();
	Button();
	Button(const Vector2& position, const UILayout* layout, 
		const map<ButtonState, unique_ptr<ImagePartSet>>& image);

	virtual bool SetResources(const wstring& filename) override { filename; return true; }
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* renderer) override;
	virtual bool IsPicking(const Vector2&)  const noexcept override { return false; }
	virtual const Rectangle& GetArea() const noexcept override;
	virtual void SetSelected(const string& name, bool selected) noexcept {};
	virtual IRenderItem* GetSelected() const noexcept { return nullptr; }
	virtual unique_ptr<IRenderItem> Clone() override;
	virtual void SetPosition(const string& name, const Vector2&) noexcept override;
	virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }
	virtual const string& GetName() const noexcept { return m_name; }
	virtual IRenderItem* GetRenderItem(const string& name) const noexcept override { return nullptr; }
	
	void SetImage(const string& name,
		const Vector2& position,
		const UILayout& layout,
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed);
	bool ChangeArea(Rectangle&& area) noexcept;
	void ChangeOrigin(Origin&& origin) noexcept;

private:
	bool SetDestination(const Rectangle& area) noexcept;

	string m_name;
	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, unique_ptr<ImagePartSet>> m_image;
	Vector2 m_position{};
};