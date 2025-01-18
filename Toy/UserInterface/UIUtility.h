#pragma once

struct SourceDivider;

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

enum class StretchType 
{
	Width,
	Height
};

struct PositionSize
{
	//Vector2 pos;
	XMINT2 pos;
	XMUINT2 size;
};

vector<PositionSize> StretchSize(StretchType stretchType, const XMUINT2& size, const vector<Rectangle>& data) noexcept;

class UILayout;
struct ImageSource;

template<typename T>
concept ImageGridClass = requires(T obj, const UILayout & layout, const ImageSource & source) {
	{ obj.SetImage(layout, source) };
};

template<ImageGridClass T>
unique_ptr<T> CreateImageGrid(const UILayout& layout, const ImageSource& source)
{
	auto imgGrid = make_unique<T>();
	if (!imgGrid->SetImage(layout, source)) return nullptr;

	return imgGrid;
}

//하나의 사각형과 두개의 점이 있을때 3개의 사각형을 찾는 함수
vector<Rectangle> GetSourcesFromArea(
	const Rectangle& area, const vector<int>& widths, const vector<int>& heights) noexcept;
Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept;
bool GetWidthsAndHeights(const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights);