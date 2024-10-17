#include "pch.h"
#include "UIUtility.h"

TextData::TextData() noexcept :
	fontStyle(L"English"), color(L"Black")
{}
TextData::TextData(const wstring& _fontStyle, const wstring& _color, const wstring& _text) noexcept :
	fontStyle(L"English"), color(L"Black"), text{ _text }
{
	if (!_fontStyle.empty()) fontStyle = _fontStyle;
	if (!_color.empty()) color = _color;
}

constexpr inline bool TextData::operator==(const TextData& rhs) const noexcept
{
	return (this->fontStyle == rhs.fontStyle && this->color == rhs.color);
}

TextProperty::TextProperty() = default;
void TextProperty::Set(const wstring& fontStyle, const wstring& color, const wstring& text) noexcept
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

bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept
{
	stack<wstring> tagStack;
	wstring fontStyle{};
	wstring color{};

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
				if (IsColor(tag)) color.clear();
				tagStack.pop();
			}
			else
			{
				tagStack.push(tag);
				if (IsFontStyle(tag)) fontStyle = tag;
				if (IsColor(tag)) color = tag;
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