#include "pch.h"
#include "UIUtility.h"

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

vector<PositionRectangle> StretchSize(const Rectangle& area, const vector<Rectangle>& data) noexcept
{
	if (data.size() != 3 && data.size() != 9) return {};	//3개짜리와 9개짜리만 취급한다.

	//필요한 데이터는 0, 2, 6 번째 데이터 뿐이다.
	vector<long> xPoints = GetStretchedSize(area.width, data.at(0).width, data.at(2).width);
	vector<long> yPoints{};
	if (data.size() == 3) yPoints = { 0, static_cast<long>(data.at(0).height) };
	if (data.size() == 9) yPoints = GetStretchedSize(area.height, data.at(0).height, data.at(6).height);

	//. . . .	. . . . 
	//. . . .	. . . .
	//. . . .
	//. . . .
	//4x4나 4x2점을 이용해서 Rectangle을 만드는 코드
	vector<Rectangle> destinations{};
	for (auto iy = yPoints.begin(); iy != prev(yPoints.end()); ++iy)
		for (auto ix = xPoints.begin(); ix != prev(xPoints.end()); ++ix)
			destinations.emplace_back(Rectangle(*ix, *iy, *(ix + 1) - *(ix), *(iy + 1) - *(iy)));

	vector<PositionRectangle> result;
	if (data.size() == 3)
	{
		for (const auto& dest : destinations)
		{
			PositionRectangle posRect;
			posRect.pos.x = float(dest.x) / float(area.width);
			posRect.pos.y = float(dest.y) / float(area.height);

			posRect.area = Rectangle(0, 0, dest.width, dest.height);
			result.emplace_back(posRect);
		}
	}

	if (data.size() == 9)
	{
		vector<PositionRectangle> res9;
		for (const auto& dest : destinations)
		{
			PositionRectangle posRect;
			posRect.pos.x = float(dest.x) / float(area.width);
			posRect.pos.y = float(dest.y) / float(area.height);

			posRect.area = Rectangle(0, 0, dest.width, dest.height);
			res9.emplace_back(posRect);
		}

		for (int idx{ 0 }; idx < res9.size(); idx += 3)
		{
			PositionRectangle posRect;
			posRect.pos = res9[idx].pos;
			long width = res9[idx + 0].area.width + res9[idx + 1].area.width + res9[idx + 2].area.width;
			long height = res9[idx].area.height;
			posRect.area = Rectangle(0, 0, width, height);
			result.emplace_back(posRect);
		}
	}

	return result;
}