#include "pch.h"
#include "TextArea.h"
#include "../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "../Config.h"
#include "JsonHelper.h"
#include "JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

//한글폰트와 영문폰트는 각각 한개만 로딩하기로 한다.
//중간에 볼드나 밑줄같은 것은 지원하지 않고 크기도 고정으로 한다.
TextArea::~TextArea() = default;
TextArea::TextArea()
{}
TextArea::TextArea(const TextArea& other)
	: UIComponent{ other }
{
	m_posByResolution = other.m_posByResolution;
	m_text = other.m_text;
	m_fontFileList = other.m_fontFileList;
	m_font = other.m_font;
	m_lines = other.m_lines;
}

bool TextArea::IsEqual(const TextArea* other) const noexcept
{
	if (!UIComponent::IsEqual(this)) return false;
	if (m_text != other->m_text) return false;
	if (m_fontFileList != other->m_fontFileList) return false;
	//나머지 멤버변수들은 이 멤버변수들을 기초로 값이 만들어진다.
	
	return true;
}

bool TextArea::LoadResources(ILoadData* load)
{
	for (const auto& file : m_fontFileList)
	{
		size_t index{ 0 };
		ReturnIfFalse(load->LoadFont(GetResourcePath() + file.second, index));
		m_font.insert(make_pair(file.first, index));
	}

	return true;
}

bool TextArea::SetDatas(IGetValue* getValue)
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(m_text, textProperty));

	auto layout = GetLayout();
	Rectangle usableArea = layout->GetArea();
	Vector2 startPos = usableArea.Location();
	float lineSpacing = 0.0f;
	long maxHeight = 0;

	vector<TextData> textList = textProperty.GetTextList();
	auto w = textList.begin();
	while (w != textList.end())
	{
		auto& word = *w;
		const auto& fontIdx = m_font[word.fontStyle];
		const Rectangle& wordRect = getValue->MeasureText(fontIdx, word.text, startPos);
		lineSpacing = max(lineSpacing, getValue->GetLineSpacing(fontIdx));
		maxHeight = max(maxHeight, wordRect.height);

		if (word.text == L"br")
		{
			startPos.x = 0.0f;
			startPos.y += lineSpacing;
			w++;
			continue;
		}

		if (usableArea.Contains(wordRect))
		{
			word.position = startPos;
			m_lines.push_back(word);
			startPos.x = static_cast<float>(wordRect.x + wordRect.width);
			w++;
			continue;
		}

		//글자 쓰는 영역을 벗어나서 더 밑으로 글자가 찍혀야 하는 경우는 글자가 있어도 강제 종료
		if (usableArea.height < wordRect.y + wordRect.height)
			break;

		//글자 쓰는 영역을 벗어나서 더 오른쪽으로 글자가 찍혀야 하는 경우 다음 줄로 개행	
		if (usableArea.width < wordRect.x + wordRect.width)
		{
			startPos.x = 0.0f;
			startPos.y += lineSpacing;
		}
	}

	return true;
}

void TextArea::SetFont(const string& name,
	const wstring& text,
	const UILayout& layout, 
	const map<wstring, wstring>& fontFileList)
{
	SetName(name);
	m_text = text;
	SetLayout(layout);
	ranges::transform(fontFileList, inserter(m_fontFileList, m_fontFileList.end()), [](const auto& filename) {
		return make_pair(filename.first, filename.second);
		});
}

bool TextArea::Update(const Vector2& position, const Mouse::ButtonStateTracker*) noexcept
{
	auto layout = GetLayout();
	m_posByResolution = layout->GetPosition(position);

	return true;
}

void TextArea::Render(IRender* render)
{
	for (const auto& word : m_lines)
		render->DrawString(m_font[word.fontStyle], 
			word.text, m_posByResolution + word.position,
			XMLoadFloat4(&word.color));
}

unique_ptr<UIComponent> TextArea::Clone() 
{
	return make_unique<TextArea>(*this);
}

void TextArea::ToJson(ordered_json& outJson) const noexcept
{
	UIComponent::ToJson(outJson);
	DataToJson("FontFileList", m_fontFileList, outJson);
	DataToJson("Text", m_text, outJson);
}

bool TextArea::Write(const wstring& filename) const noexcept
{
	ordered_json j;
	ToJson(j);
	return WriteJsonAA(j, filename);
}

bool TextArea::FromJson(const json& j) noexcept
{
	UIComponent::FromJson(j); 
	DataFromJson("FontFileList", m_fontFileList, j);
	DataFromJson("Text", m_text, j);

	return true;
}

bool TextArea::Read(const wstring& filename) noexcept
{
	const json& j = ReadJsonAA(filename);
	if (j.is_null()) return false;

	return FromJson(j);
}

void TextArea::Process(JsonOperation* operation) noexcept
{
	operation->Process("FontFileList", m_fontFileList);
	operation->Process("Text", m_text);
}