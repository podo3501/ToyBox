#include "pch.h"
#include "TextArea.h"
#include "../../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "../Config.h"

//�ѱ���Ʈ�� ������Ʈ�� ���� �Ѱ��� �ε��ϱ�� �Ѵ�.
//�߰��� ���峪 ���ٰ��� ���� �������� �ʰ� ũ�⵵ �������� �Ѵ�.
TextArea::~TextArea() = default;
TextArea::TextArea() :
	m_layout{ nullptr }
{}

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

const Rectangle& TextArea::GetArea() const noexcept 
{ 
	return m_layout->GetArea(); 
}

void TextArea::SetFont(IRenderer* renderer, 
	const Vector2& position,
	const UILayout& layout, 
	const map<wstring, wstring>& fontFileList)
{
	m_position = position;
	m_layout = make_unique<UILayout>(layout);
	ranges::transform(fontFileList, inserter(m_fontFileList, m_fontFileList.end()), [](const auto& filename) {
		return make_pair(filename.first, filename.second);
		});

	renderer->AddLoadResource(this);
}

bool TextArea::SetText(IGetValue* getValue, wstring&& text)
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(text, textProperty));

	Rectangle usableArea = m_layout->GetArea();
	Vector2 startPos = usableArea.Location();
	float lineSpacing = 0.0f;
	long maxHeight = 0;

	vector<TextData> textList = textProperty.GetTextList();
	auto w = textList.begin();
	while(w != textList.end())
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

bool TextArea::Update(const Vector2& position) noexcept
{
	m_position = m_layout->GetPosition(m_position + position);

	return true;
}

void TextArea::Render(IRender* render)
{
	for (const auto& word : m_lines)
		render->DrawString(m_font[word.fontStyle], 
			word.text, m_position + word.position, 
			XMLoadFloat4(&word.color));
}

TextArea::TextArea(const Vector2& position, const UILayout* layout,
	const map<wstring, wstring>& fontFileList, 
	const map<wstring, size_t>& font, 
	const vector<TextData>& lines)
{
	m_position = position;
	m_layout = make_unique<UILayout>(*layout);
	m_fontFileList = fontFileList;
	m_font = font;
	m_lines = lines;
}

unique_ptr<IRenderItem> TextArea::Clone() 
{
	return make_unique<TextArea>(m_position, m_layout.get(), m_fontFileList, m_font, m_lines);
}

void TextArea::SetPosition(const string& name, const Vector2& position) noexcept 
{
	m_position = position;
};