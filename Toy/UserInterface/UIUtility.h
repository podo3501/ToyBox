#pragma once

struct SourceDivider;
class UIComponent;
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

//L"<Hangle><Red>�׽�Ʈ, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>"
bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept;

vector<XMUINT2> StretchSize(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept;
vector<XMINT2> ExtractStartPosFromSizes(DirectionType dirType, const vector<XMUINT2>& sizes) noexcept;
bool IsBiggerThanSource(DirectionType dirType, const XMUINT2& size, const vector<Rectangle>& list);
//�ϳ��� �簢���� �ΰ��� ���� ������ 3���� �簢���� ã�� �Լ�
vector<Rectangle> GetSourcesFromArea(
	const Rectangle& area, const vector<int>& widths, const vector<int>& heights) noexcept;
Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept;
bool GetSizeDividedByThree(DirectionType type, const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights) noexcept;
bool GetSizeDividedByNine(const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights) noexcept;