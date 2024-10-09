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

	//��ư�� 3���� �������� �ִ�. ��ư ����� �ø��� �߰��� �þ��.
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
		const vector<ImageSource>& normal,
		const vector<ImageSource>& hover,
		const vector<ImageSource>& pressed,
		const Rectangle& area, const Vector2& pos, Origin origin);
	void Update(const Vector2& resolution, const Mouse::ButtonStateTracker& tracker);
	void ChangeOrigin(Origin origin);

private:
	void SetLocalPosition();

	void SetImage(const wstring& resPath, ButtonState btnState, const vector<ImageSource>& sources);
	bool LoadResources(ILoadData* load, const vector<unique_ptr<ImagePart>>& imageParts);

	void EachImageParts(ButtonState btnState, function<void(ImagePart*)> action);
	void EachImageParts(Part part, function<void(ImagePart*)> action);
	void EachImageParts(function<void(ImagePart*)> action) const;
	

	unique_ptr<UILayout> m_layout;
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, vector<unique_ptr<ImagePart>>> m_NbuttonParts;
};