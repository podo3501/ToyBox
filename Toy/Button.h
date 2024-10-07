#pragma once

#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

class Texture;
class ImagePart;
class UILayout;

enum class Origin;

struct ButtonImage
{
public:
	std::vector<std::wstring> filenames;
};

class Button : public IRenderItem
{
	enum class ButtonState
	{
		Normal,
		Over,
		Clicked,
		Count,
	};

	//버튼은 3개로 나누어져 있다. 버튼 사이즈를 늘리면 중간만 늘어난다.
	enum Part
	{
		Left = 0,
		Middle = 1,
		Right = 2,
	};

public:
	Button(const std::wstring& resPath);
	virtual ~Button();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;
	
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const Rectangle& area, const Vector2& pos, Origin origin);
	void Update(const Vector2& resolution, const Mouse::State& state);
	void ChangeOrigin(Origin origin);

private:
	bool LoadTextures(ILoadData* load, const vector<unique_ptr<ImagePart>>& imageParts);
	void SetFilenames(ButtonState state, const ButtonImage& images);

	inline void SetArea(const Rectangle& area) { m_area = area; }
	inline void SetPosition(const XMFLOAT2& pos) { m_position = pos; }
	void SetOrigin(Origin origin);
	Vector2 GetOrigin(Origin origin) const noexcept;

	wstring m_resPath{};
	unique_ptr<UILayout> m_layout;
	Rectangle m_area{};
	Vector2 m_position{};
	Vector2 m_origin{};
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, vector<unique_ptr<ImagePart>>> m_buttonParts;
};