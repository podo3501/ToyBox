#include "pch.h"
#include "TextArea.h"
#include "../Include/IRenderer.h"
#include "../../Utility.h"
#include "../../Config.h"
#include "../UIUtility.h"
#include "../JsonOperation.h"
#include "../UIComponentEx.h"

using namespace UIComponentEx;

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

//한글폰트와 영문폰트는 각각 한개만 로딩하기로 한다.
//중간에 볼드나 밑줄같은 것은 지원하지 않고 크기도 고정으로 한다.
TextArea::~TextArea() = default;
TextArea::TextArea()
{}

TextArea::TextArea(const TextArea& other) :
	UIComponent{ other }
{
	m_posByResolution = other.m_posByResolution;
	m_text = other.m_text;
	m_fontFileList = other.m_fontFileList;
	m_font = other.m_font;
	m_lines = other.m_lines;
}

unique_ptr<UIComponent> TextArea::CreateClone() const
{
	return unique_ptr<TextArea>(new TextArea(*this));
}

bool TextArea::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const TextArea* rhs = static_cast<const TextArea*>(&o);
	
	return tie(m_fontFileList, m_text) == tie(rhs->m_fontFileList, rhs->m_text);
}

bool TextArea::LoadResources(ILoadData* load)
{
	for (const auto& file : m_fontFileList)
	{
		size_t index{ 0 };
		ReturnIfFalse(load->LoadFont(GetResourceFullFilename(file.second), index));
		m_font.insert(make_pair(file.first, index));
	}

	return true;
}

bool TextArea::SetDatas(IGetValue* getValue)
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(m_text, textProperty));

	Rectangle usableArea = XMUINT2ToRectangle(GetSize(this));
	Vector2 startPos{};
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

void TextArea::SetFont(const wstring& text, const UILayout& layout, const map<wstring, wstring>& fontFileList) noexcept
{
	m_text = text;
	SetLayout(layout);
	ranges::transform(fontFileList, inserter(m_fontFileList, m_fontFileList.end()), [](const auto& filename) {
		return make_pair(filename.first, filename.second);
		});
}

bool TextArea::ImplementUpdate(const XMINT2& position) noexcept
{
	if (IsDirty())
		m_posByResolution = XMINT2ToVector2(GetPositionByLayout(position));

	return true;
}

void TextArea::ImplementRender(IRender* render) const
{
	for (const auto& word : m_lines)
		render->DrawString(m_font.at(word.fontStyle), 
			word.text, m_posByResolution + word.position,
			XMLoadFloat4(&word.color));
}

void TextArea::SerializeIO(JsonOperation& operation)
{
	operation.Process("FontFileList", m_fontFileList);
	operation.Process("Text", m_text);
	UIComponent::SerializeIO(operation);
}

unique_ptr<TextArea> CreateTextArea(const UILayout& layout,
	const wstring& text, map<wstring, wstring>& fontFileList)
{
	unique_ptr<TextArea> textArea = make_unique<TextArea>();
	textArea->SetFont(text, layout, fontFileList);

	return textArea;
}
