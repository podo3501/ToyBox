#pragma once

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

//L"<Hangle><Red>Å×½ºÆ®, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>"
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
concept ImageGridClass = requires(T obj, const string & name, const UILayout & layout, const ImageSource & source) {
	{ obj.SetImage(name, layout, source) };
};

template<ImageGridClass T>
unique_ptr<T> CreateImageGrid(const string& name, const UILayout& layout, const ImageSource& source)
{
	auto imgGrid = make_unique<T>();
	if (!imgGrid->SetImage(name, layout, source)) return nullptr;

	return imgGrid;
}

vector<Rectangle> GetSourcesFromAreaAndGaps(const Rectangle& area, const vector<int>& widths) noexcept;
