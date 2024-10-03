#pragma once

#include "../Include/IRenderItem.h"

class Texture;
struct IRenderer;
struct ILoadData;

using DirectX::SimpleMath::Vector2;

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
	DirectX::XMINT2 size{};
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
	
	void Update(const DirectX::SimpleMath::Vector2& resolution, const DirectX::Mouse::State& state);
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const DirectX::XMUINT2& size, const DirectX::XMFLOAT2& pos, Origin origin);
	void SetOrigin(Origin origin);

	inline void SetSize(const DirectX::XMUINT2& size);
	inline void SetPosition(const DirectX::XMFLOAT2& pos);

private:
	void LoadTextures(ILoadData* load, ButtonImage& images);

	std::wstring m_resPath{};
	std::map<ButtonState, ButtonImage> m_images;
	DirectX::XMUINT2 m_size{};
	DirectX::XMFLOAT2 m_position{};
	DirectX::XMFLOAT2 m_origin{};
	DirectX::XMFLOAT2 m_sidePosition{};
	//Origin m_origin;
	ButtonState m_state{ ButtonState::Normal };
};

inline void Button::SetSize(const DirectX::XMUINT2& size) { m_size = size; }
inline void Button::SetPosition(const DirectX::XMFLOAT2& pos) { m_position = pos; }
