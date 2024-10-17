#pragma once

struct TextData
{
	TextData() noexcept;
	TextData(const wstring& _fontStyle, const wstring& _color, const wstring& text) noexcept;

	constexpr inline bool operator==(const TextData& rhs) const noexcept;

	wstring fontStyle;
	wstring color;
	wstring text;
};

class TextProperty
{
public:
	TextProperty();
	void Set(const wstring& fontStyle, const wstring& color, const wstring& text) noexcept;
	void AddText(const wstring& text) noexcept;
	inline const vector<TextData>& GetTextList() const noexcept { return m_data; }

private:
	vector<TextData> m_data;
};

//L"<Hangle><Red>Å×½ºÆ®, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>"
bool Parser(const wstring& context, TextProperty& outTextProperty) noexcept;
