#pragma once

#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePart;
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

	//버튼은 3개로 나누어져 있다. 버튼 사이즈를 늘리면 중간만 늘어난다.
	enum Part
	{
		Left = 0,
		Center = 1,
		Right = 2,
	};

public:
	Button();
	virtual ~Button();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(
		const wstring& resPath,
		const vector<ImageSource>& left,
		const vector<ImageSource>& center,
		const vector<ImageSource>& right,
		const Rectangle& area, const Vector2& pos, Origin origin);
	void Update(const Vector2& resolution, const Mouse::ButtonStateTracker& tracker);
	void ChangeOrigin(Origin origin);

private:
	void SetLocalPosition();

	void SetImagePart(Part part, const ImageSource& patch);

	void EachImageParts(function<void(ImagePart*)> action) const;
	bool EachImageParts(function<bool(ImagePart*)> action);
	void EachImageParts(Part part, function<void(ImagePart*)> action);
	void EachImageParts(ButtonState btnState, function<void(ImagePart*)> action);

	void SetImagePart(const wstring& resPath, Part part, const ImageSource& source);

	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<Part, vector<unique_ptr<ImagePart>>> m_buttonParts;
	//map<ButtonState, vector<unique_ptr<ImagePart>>> m_buttonParts;

	//vector<ImageParts> m_buttonParts;
};