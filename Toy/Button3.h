#pragma once

#include "../Include/IRenderItem.h"

class Texture;
struct IRenderer;
struct ILoadData;

struct ButtonImage
{
public:
	int heapIndex;
	std::vector<std::wstring> filenames;
	DirectX::XMINT2 size{};
	std::vector<DirectX::SimpleMath::Vector2> position;
};

class Button3 : public IRenderItem
{
	enum class ButtonState
	{
		Normal,
		Over,
		Clicked,
		Count,
	};

public:
	Button3(const std::wstring& resPath, const DirectX::SimpleMath::Vector2& resolution);
	virtual ~Button3();

	virtual void LoadResources(ILoadData* load) override;
	virtual void Render(IRenderer* renderer) override;
	
	void Update(const DirectX::Mouse::State& state);
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const DirectX::SimpleMath::Vector2& pos);

private:
	void LoadTextures(ILoadData* load, ButtonImage& images);

	std::wstring m_resPath{};
	DirectX::SimpleMath::Vector2 m_resolution{};
	std::map<ButtonState, ButtonImage> m_images;
	DirectX::SimpleMath::Vector2 m_position{};
	ButtonState m_state{ ButtonState::Normal };
	DirectX::SimpleMath::Vector2 m_origin{};
};
