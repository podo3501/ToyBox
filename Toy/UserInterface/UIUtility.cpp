#include "pch.h"
#include "UIUtility.h"
#include "UIType.h"

bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

TextData::TextData() noexcept :
	fontStyle(L"English"), color{}
{
	XMStoreFloat4(&color, Colors::Black);
}

bool IsZero(const XMFLOAT4& color) 
{
	return color.x == 0.0f && color.y == 0.0f && color.z == 0.0f && color.w == 0.0f;
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
	if (color == L"Red" || color == L"Blue") return true;
	return false;
}

XMFLOAT4 GetColor(const wstring& color) noexcept
{
	XMVECTORF32 vColor = Colors::Black;
	if (color == L"Red") vColor = Colors::Red;
	if (color == L"Black") vColor = Colors::Black;
	if (color == L"Blue") vColor = Colors::Blue;

	XMFLOAT4 fColor;
	XMStoreFloat4(&fColor, vColor);
	return fColor;
}

bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept
{
	stack<wstring> tagStack;
	wstring fontStyle{};
	XMFLOAT4 color{};

	auto c = context.begin();
	while (c != context.end())
	{
		//tag처리
		if (*c == '<') {
			auto tagStart = c + 1;
			auto tagEnd = find(c, context.end(), '>');
			wstring tag = move(wstring(tagStart, tagEnd));
			if (tag[0] == '/')
			{
				tag.erase(0, 1);
				if (tagStack.top() != tag)
					return false;
				if (IsFontStyle(tag)) fontStyle.clear();
				if (IsColor(tag)) color = {};
				tagStack.pop();
			}
			else if (tag == L"br")	//줄바꿈. L"br"로 저장하고 나중에 줄바꿈 처리한다.
			{
				outTextProperty.Set(fontStyle, color, tag);
			}
			else
			{
				tagStack.push(tag);
				if (IsFontStyle(tag)) fontStyle = tag;
				if (IsColor(tag)) color = GetColor(tag);
			}
			c = tagEnd + 1;
			continue;	//tag가 연속적으로 나올 수 있기 때문에 처음으로 돌아간다.
		}

		size_t start = distance(context.begin(), c);
		size_t end = context.find_first_of(L" <", start);

		size_t dist = end - start;
		outTextProperty.Set(fontStyle, color, context.substr(start, dist));
		c += dist;

		if ((*c) == ' ')	//공백은 화면에 출력해야 한다.
		{
			outTextProperty.AddText(L" ");
			c++;
		}
	}

	if (!tagStack.empty()) return false;

	return true;
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

vector<PositionSize> StretchSize(StretchType stretchType, const XMUINT2& size, const vector<Rectangle>& data) noexcept
{
	if (data.size() != 3) return {};

	vector<long> xPoints, yPoints;

	switch (stretchType) {
	case StretchType::Width:
		// 가로 확장
		xPoints = GetStretchedSize(size.x, data[0].width, data[2].width);
		yPoints = { 0, static_cast<long>(size.y) };
		break;
	case StretchType::Height:
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

vector<Rectangle> GetSourcesFromArea(const Rectangle& area, const vector<int>& widths, const vector<int>& heights) noexcept
{
	if (area.IsEmpty() || widths.empty() || heights.empty()) {
		return {};
	}

	vector<Rectangle> areas;
	long currentY = area.y;
	
	for (auto heightIndex : std::views::iota(size_t{ 0 }, heights.size())) 
	{
		long currentX = area.x;  
		for (auto widthIndex : std::views::iota(size_t{ 0 }, widths.size())) 
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

bool GetWidthsAndHeights(const SourceDivider& srcDivider, vector<int>& outWidths, vector<int>& outHeights)
{
	if (srcDivider.list.size() != 2 && srcDivider.list.size() != 4)
		return false;

	auto getDivisions = [](const vector<int>& points, int totalSize) -> vector<int> {
		vector<int> divisions = points;
		divisions.push_back(totalSize);
		ranges::sort(divisions);

		return { divisions[0], divisions[1] - divisions[0], divisions[2] - divisions[1] };
		};

	// Width 계산
	outWidths = getDivisions({ srcDivider.list.begin(), srcDivider.list.begin() + 2 }, srcDivider.rect.width);

	if (srcDivider.list.size() == 2) {
		outHeights = { srcDivider.rect.height };
		return true;
	}

	// Height 계산 (srcDivider.list가 4일 때만)
	outHeights = getDivisions({ srcDivider.list.begin() + 2, srcDivider.list.begin() + 4 }, srcDivider.rect.height);

	return true;
}
