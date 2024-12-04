#include "pch.h"
#include "TextArea.h"
#include "../Include/IRenderer.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "../Config.h"
#include "UILayout.h"
#include "UIUtility.h"
#include "JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

//�ѱ���Ʈ�� ������Ʈ�� ���� �Ѱ��� �ε��ϱ�� �Ѵ�.
//�߰��� ���峪 ���ٰ��� ���� �������� �ʰ� ũ�⵵ �������� �Ѵ�.
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

unique_ptr<UIComponent> TextArea::Clone()
{
	auto clone = make_unique<TextArea>(*this);
	clone->SetName(clone->GetName() + "_clone");
	return clone;
}

TextArea& TextArea::operator=(const TextArea& o)
{
	if (this == &o)
		return *this;

	UIComponent::operator=(o);
	tie(m_fontFileList, m_text) = tie(o.m_fontFileList, o.m_text);

	return *this;
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

		//���� ���� ������ ����� �� ������ ���ڰ� ������ �ϴ� ���� ���ڰ� �־ ���� ����
		if (usableArea.height < wordRect.y + wordRect.height)
			break;

		//���� ���� ������ ����� �� ���������� ���ڰ� ������ �ϴ� ��� ���� �ٷ� ����	
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

bool TextArea::Update(const XMINT2& position, MouseTracker*) noexcept
{
	m_posByResolution = XMINT2ToVector2(GetPositionByLayout(position));

	return true;
}

void TextArea::Render(IRender* render)
{
	for (const auto& word : m_lines)
		render->DrawString(m_font[word.fontStyle], 
			word.text, m_posByResolution + word.position,
			XMLoadFloat4(&word.color));
}

void TextArea::SerializeIO(JsonOperation& operation)
{
	operation.Process("FontFileList", m_fontFileList);
	operation.Process("Text", m_text);
	UIComponent::SerializeIO(operation);
}
