#pragma once

#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

class Texture;
class ImagePart;

enum class Origin
{
	Center,
	LeftTop,
};

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
	
	void Update(const Vector2& resolution, const Mouse::State& state);
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const Rectangle& area, const Vector2& pos, Origin origin);

	inline void SetArea(const Rectangle& area) { m_area = area; }
	inline void SetPosition(const XMFLOAT2& pos) { m_position = pos; }
	void SetOrigin(Origin origin);

private:
	bool LoadTextures(ILoadData* load, const vector<unique_ptr<ImagePart>>& imageParts);
	void SetFilenames(ButtonState state, const ButtonImage& images);
	Vector2 GetOrigin(Origin origin) const noexcept;

	wstring m_resPath{};
	Rectangle m_area{};
	Vector2 m_position{};
	Vector2 m_origin{};
	ButtonState m_state{ ButtonState::Normal };
	map<ButtonState, vector<unique_ptr<ImagePart>>> m_buttonParts;
};