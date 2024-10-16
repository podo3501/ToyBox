#pragma once

struct TextData
{
	TextData() noexcept;
	TextData(const wstring& _fontStyle, const wstring& _color) noexcept;

	constexpr inline bool operator==(const TextData& rhs) const noexcept;

	wstring fontStyle;
	wstring color;
	wstring text;
};

class TextProperty
{
public:
	TextProperty();
	void Set(const wstring& fontStyle, const wstring& color) noexcept;
		void AddCharacter(const wchar_t& c) noexcept;

private:
	vector<TextData> m_data;
};

//L"<Hangle><Red>Å×½ºÆ®, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>"
bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept;

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
