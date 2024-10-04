#pragma once

#include "../Include/IRenderItem.h"

class Texture;
struct IRenderer;
struct ILoadData;

enum class Origin
{
	Center,
	LeftTop,
};

struct ButtonImage
{
public:
	int heapIndex;
	std::vector<std::wstring> filenames;
	std::vector<Vector2> position;
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

public:
	Button(const std::wstring& resPath);
	virtual ~Button();

	virtual void LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;
	
	void Update(const Vector2& resolution, const Mouse::State& state);
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const Rectangle& area, const Vector2& pos, Origin origin);
	void SetOrigin(Origin origin);

	inline void SetArea(const Rectangle& area) { m_area = area; }
	inline void SetPosition(const XMFLOAT2& pos) { m_position = pos; }

private:
	void LoadTextures(ILoadData* load, ButtonImage& images);

	std::wstring m_resPath{};
	std::map<ButtonState, ButtonImage> m_images;
	Rectangle m_area{};
	Vector2 m_position{};
	Vector2 m_origin{};
	Vector2 m_sidePosition{};
	ButtonState m_state{ ButtonState::Normal };
};