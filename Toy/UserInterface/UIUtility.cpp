#include "pch.h"
#include "UIUtility.h"

TextData::TextData() noexcept :
	fontStyle(L"English"), color(L"Black")
{}
TextData::TextData(const wstring& _fontStyle, const wstring& _color) noexcept :
	fontStyle(L"English"), color(L"Black")
{
	if (!_fontStyle.empty()) fontStyle = _fontStyle;
	if (!_color.empty()) color = _color;
}

constexpr inline bool TextData::operator==(const TextData& rhs) const noexcept
{
	return (this->fontStyle == rhs.fontStyle && this->color == rhs.color);
}

TextProperty::TextProperty() = default;
void TextProperty::Set(const wstring& fontStyle, const wstring& color) noexcept
{
	TextData textData(fontStyle, color);
	if (!m_data.empty() && *m_data.rbegin() == textData)
		return;

	m_data.emplace_back(textData);
}

void TextProperty::AddCharacter(const wchar_t& c) noexcept
{
	m_data.rbegin()->text += c;
}

bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept
{
	stack<wstring> tagStack;
	wstring fontStyle{ L"English" };
	wstring color{ L"Black" };

	auto c = context.begin();
	while (c != context.end())
	{
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
			continue;
		}

		outTextProperty.Set(fontStyle, color);
		outTextProperty.AddCharacter(*c);
		c++;
	}

	if (!tagStack.empty()) return false;

	return true;
}