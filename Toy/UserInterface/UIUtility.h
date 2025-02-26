#pragma once

struct SourceDivider;
enum class DirectionType;

struct TextData
{
	TextData() noexcept;
	TextData(const wstring& _fontStyle, const XMFLOAT4& _color, const wstring& _text) noexcept;

	wstring fontStyle;
	XMFLOAT4 color;
	wstring text;
	Vector2 position{};
};

class TextProperty
{
public:
	TextProperty();
	void Set(const wstring& fontStyle, const XMFLOAT4& color, const wstring& text) noexcept;
	void AddText(const wstring& text) noexcept;
	inline vector<TextData> GetTextList() const noexcept { return m_data; }

private:
	vector<TextData> m_data;
};

//L"<Hangle><Red>테스트, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>"
bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept;

struct PositionSize
{
	//Vector2 pos;
	XMINT2 pos;
	XMUINT2 size;
};

vector<PositionSize> StretchSize(DirectionType stretchType, const XMUINT2& size, const vector<Rectangle>& data) noexcept;

//하나의 사각형과 두개의 점이 있을때 3개의 사각형을 찾는 함수
vector<Rectangle> GetSourcesFromArea(
	const Rectangle& area, const vector<int>& widths, const vector<int>& heights) noexcept;
Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept;
bool GetSizeDividedByThree(DirectionType type, const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights) noexcept;
bool GetSizeDividedByNine(const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights) noexcept;