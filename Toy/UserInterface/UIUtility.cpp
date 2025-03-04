#include "pch.h"
#include "UIUtility.h"
#include "UIType.h"

static inline bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) noexcept
{
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

static inline bool IsZero(const XMFLOAT4& color) noexcept
{
	return color.x == 0.0f && color.y == 0.0f && color.z == 0.0f && color.w == 0.0f;
}

TextData::TextData() noexcept :
	fontStyle(L"English"), color{}
{
	XMStoreFloat4(&color, Colors::Black);
}

TextData::TextData(const wstring& _fontStyle, const XMFLOAT4& _color, const wstring& _text) noexcept :
	fontStyle(L"English"), color{}, text{ _text }
{
	XMStoreFloat4(&color, Colors::Black);

	if (!_fontStyle.empty()) fontStyle = _fontStyle;
	if (!IsZero(_color)) color = _color;
}

TextProperty::TextProperty() = default;
void TextProperty::Set(const wstring& fontStyle, const XMFLOAT4& color, const wstring& text) noexcept
{
	m_data.emplace_back(move(TextData{ fontStyle, color, text }));
}

void TextProperty::AddText(const wstring& text) noexcept
{
	TextData& last = *(m_data.rbegin());
	last.text += text;
}

inline constexpr bool IsFontStyle(const wstring& fontStyle) noexcept
{
	if (fontStyle == L"Hangle" || fontStyle == L"English") return true;
	return false;
}

inline constexpr bool IsColor(const wstring& color) noexcept
{
	if (color == L"Red" || color == L"Blue" || color == L"White" || color == L"Black") return true;
	return false;
}

XMFLOAT4 GetColor(const wstring& color) noexcept
{
	static const unordered_map<wstring, XMVECTORF32> colorMap = {
		{L"White", Colors::White},
		{L"Red", Colors::Red},
		{L"Black", Colors::Black},
		{L"Blue", Colors::Blue}
	};

	auto it = colorMap.find(color);
	XMVECTORF32 vColor = (it != colorMap.end()) ? it->second : Colors::Black;

	XMFLOAT4 fColor;
	XMStoreFloat4(&fColor, vColor);
	return fColor;
}

bool ProcessTag(const wstring& tag, stack<wstring>& tagStack, wstring& fontStyle, XMFLOAT4& color)
{
	if (tag.empty()) return false;

	if (tag[0] == '/') // 닫는 태그
	{
		wstring closingTag = tag.substr(1);
		if (tagStack.empty() || tagStack.top() != closingTag)
			return false; // 태그 불일치

		tagStack.pop();
		if (IsFontStyle(closingTag)) fontStyle.clear();
		if (IsColor(closingTag)) color = {};
	}
	else if (tag == L"br") // 줄바꿈 태그
	{
		return true; // 줄바꿈을 별도로 처리
	}
	else // 여는 태그
	{
		tagStack.push(tag);
		if (IsFontStyle(tag)) fontStyle = tag;
		if (IsColor(tag)) color = GetColor(tag);
	}

	return true;
}

bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept
{
	stack<wstring> tagStack;
	wstring fontStyle;
	XMFLOAT4 color;

	auto c = context.begin();
	while (c != context.end())
	{
		if (*c == '<') // 태그 시작
		{
			auto tagStart = c + 1;
			auto tagEnd = find(c, context.end(), '>');
			if (tagEnd == context.end()) return false; // 잘못된 태그

			wstring tag(tagStart, tagEnd);
			c = tagEnd + 1; // 태그 끝 다음 문자로 이동

			if (!ProcessTag(tag, tagStack, fontStyle, color))
				return false; // 태그 처리 실패 시 종료

			if (tag == L"br")
				outTextProperty.Set(fontStyle, color, tag);

			continue; // 태그 처리 후 다음 루프로 이동
		}

		// 일반 텍스트 처리
		size_t start = distance(context.begin(), c);
		size_t end = context.find_first_of(L" <", start);

		if (end == wstring::npos) // <태크를 못 찾으면 이상한 것
			return false;

		outTextProperty.Set(fontStyle, color, context.substr(start, end - start));
		c = context.begin() + end; // 커서 위치 업데이트

		if (*c == ' ') // 공백 처리
		{
			outTextProperty.AddText(L" ");
			++c;
		}
	}

	return tagStack.empty(); // 태그가 남아 있으면 false
}

//양 끝단을 빼고 중간길이를 구한다음 총 4점을 리턴한다.
//가로 4점 세로2점이면 3개의 Rectangle을 구할 수 있다.
//중간에 한쪽 방향으로만 늘어나고 옆으로 늘릴때에는 높이, 위아래로 늘릴때는 넓이가 일정하다고 가정한다.
static vector<long> GetStretchedSize(long length, long thisEdge, long thatEdge) noexcept
{
	long middle = 0;
	if (length > thisEdge + thatEdge)
		middle = length - (thisEdge + thatEdge);

	return { 0, thisEdge, thisEdge + middle, length };
}

vector<PositionSize> StretchSize(DirectionType stretchType, const XMUINT2& size, const vector<Rectangle>& data) noexcept
{
	if (data.size() != 3) return {};

	vector<long> xPoints, yPoints;

	switch (stretchType) {
	case DirectionType::Horizontal:
		// 가로 확장
		xPoints = GetStretchedSize(size.x, data[0].width, data[2].width);
		yPoints = { 0, static_cast<long>(size.y) };
		break;
	case DirectionType::Vertical:
		// 세로 확장
		yPoints = GetStretchedSize(size.y, data[0].height, data[2].height);
		xPoints = { 0, static_cast<long>(size.x) };
		break;
	}

	// 결과 리스트 생성
	vector<PositionSize> result;
	result.reserve((xPoints.size() - 1) * (yPoints.size() - 1)); // 메모리 예약

	// PositionRectangle 생성
	for (size_t iy = 0; iy < yPoints.size() - 1; ++iy) {
		for (size_t ix = 0; ix < xPoints.size() - 1; ++ix) {
			result.emplace_back(PositionSize{
				{ static_cast<int32_t>(xPoints[ix]), static_cast<int32_t>(yPoints[iy]) },
				{	
					static_cast<uint32_t>(xPoints[ix + 1] - xPoints[ix]),
					static_cast<uint32_t>(yPoints[iy + 1] - yPoints[iy]) 
				}
			});
		}
	}

	return result;
}

bool IsBiggerThanSource(DirectionType dirType, const XMUINT2& size, const vector<Rectangle>& list)
{
	uint32_t sizeValue{ 0 };
	uint32_t sourceSum{ 0 };
	function<int(uint32_t, const Rectangle&)> accumulator;

	switch (dirType) {
	case DirectionType::Horizontal:
		accumulator = [](uint32_t sum, const Rectangle& rect) { return sum + rect.width; };
		sizeValue = size.x;
		break;
	case DirectionType::Vertical:
		accumulator = [](uint32_t sum, const Rectangle& rect) { return sum + rect.height; };
		sizeValue = size.y;
		break;
	}

	sourceSum = accumulate(list.begin(), list.end(), 0, accumulator);
	return sourceSum <= sizeValue;
}


vector<Rectangle> GetSourcesFromArea(const Rectangle& area, const vector<int>& widths, const vector<int>& heights) noexcept
{
	if (area.IsEmpty() || widths.empty() || heights.empty()) {
		return {};
	}

	vector<Rectangle> areas;
	long currentY = area.y;
	
	for (auto heightIndex : views::iota(size_t{ 0 }, heights.size())) 
	{
		long currentX = area.x;  
		for (auto widthIndex : views::iota(size_t{ 0 }, widths.size())) 
		{
			areas.emplace_back(currentX, currentY, widths[widthIndex], heights[heightIndex]);
			currentX += widths[widthIndex];  
		}
		currentY += heights[heightIndex];  
	}

	return areas;
}

Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept
{
	if (rectangles.empty())
		return {};

	Rectangle result = rectangles[0];
	for (const Rectangle& rect : rectangles)
		result = Rectangle::Union(result, rect);

	return result;
}

static vector<int> GetDivisions(const vector<int>& points, int totalSize) noexcept
{
	vector<int> divisions = points;
	divisions.push_back(totalSize);
	ranges::sort(divisions);
	return { divisions[0], divisions[1] - divisions[0], divisions[2] - divisions[1] };
}

bool GetSizeDividedByThree(DirectionType type, const SourceDivider& srcDivider,
	vector<int>& outWidths, vector<int>& outHeights) noexcept
{
	const auto& divideList = srcDivider.list;
	const auto& rect = srcDivider.rect;
	if (divideList.size() != 2) return false;

	switch (type)
	{
	case DirectionType::Horizontal:
		outWidths = GetDivisions({ divideList.begin(), divideList.begin() + 2 }, rect.width);
		outHeights = { rect.height };
		break;
	case DirectionType::Vertical:
		outWidths = { rect.width };
		outHeights = GetDivisions({ divideList.begin(), divideList.begin() + 2 }, rect.height);
		break;
	}
	return true;
}

bool GetSizeDividedByNine(const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights) noexcept
{
	const auto& divideList = srcDivider.list;
	const auto& rect = srcDivider.rect;
	if (divideList.size() != 4) return false;

	outWidths = GetDivisions({ divideList.begin(), divideList.begin() + 2 }, rect.width);
	outHeights = GetDivisions({ divideList.begin() + 2, divideList.begin() + 4 }, rect.height);
	return true;
}