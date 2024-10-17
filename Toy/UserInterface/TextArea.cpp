#include "pch.h"
#include "TextArea.h"
#include "../../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"
#include "UIUtility.h"

struct WordPosition
{
	WordPosition(const TextData& _text, const Vector2& _position)
	{
		fontStyle = _text.fontStyle;
		color = _text.color;
		text = _text.text;
		position = _position;
	}

	wstring fontStyle{};
	wstring color{};
	wstring text{};
	Vector2 position{};
};

class Sentence
{
public:
	Sentence() = default;
	~Sentence() = default;

	bool Load(ILoadData* load, const wstring& filename)
	{
		return load->LoadFont(filename, m_index);
	}

	Rectangle MeasureText(IUpdate* update, const wstring& text, const Vector2& position) const
	{
		return update->MeasureText(m_index, text, position);
	}

	float GetLineSpacing(IUpdate* update)
	{
		return update->GetLineSpacing(m_index);
	}

	void DrawString(IRender* render, const XMUINT2& position, const WordPosition& word) const
	{
		XMVECTORF32 color = Colors::Black;
		if (word.color == L"Red")
			color = Colors::Red;
		if (word.color == L"Black")
			color = Colors::Black;
		if (word.color == L"Blue")
			color = Colors::Blue;

		XMUINT2 curPos{
			position.x + static_cast<uint32_t>(word.position.x),
			position.y + static_cast<uint32_t>(word.position.y) };
		render->DrawString(m_index, word.text, { static_cast<float>(curPos.x), static_cast<float>(curPos.y) }, color);
	}

private:
	size_t m_index{ 0 };
};

//�ѱ���Ʈ�� ������Ʈ�� ���� �Ѱ��� �ε��ϱ�� �Ѵ�.
//�߰��� ���峪 ���ٰ��� ���� �������� �ʰ� ũ�⵵ �������� �Ѵ�.
//���߿� ����ϵ��� �Ѵ�.
TextArea::TextArea() :
	m_layout{ nullptr }
{}

bool TextArea::LoadResources(ILoadData* load)
{
	for (const auto& file : m_fontFileList)
	{
		auto sentence = make_unique<Sentence>();
		ReturnIfFalse(sentence->Load(load, file.second));
		m_sentences.insert(make_pair(file.first, move(sentence)));
	}

	return true;
}

void TextArea::SetFont(const wstring& resPath, const map<wstring, wstring>& fontFileList, const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);
	ranges::transform(fontFileList, inserter(m_fontFileList, m_fontFileList.end()), [&resPath](const auto& filename) {
		return make_pair(filename.first, resPath + filename.second);
		});
}

bool TextArea::SetText(IUpdate* update, wstring&& text)
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(text, textProperty));

	Rectangle usableArea = m_layout->GetArea();
	Vector2 startPos = { static_cast<float>(usableArea.x), static_cast<float>(usableArea.y) };
	float lineSpacing = 0.0f;
	long maxHeight = 0;
	auto w = textProperty.GetTextList().begin();
	while(w != textProperty.GetTextList().end())
	{
		const auto& word = *w;
		const auto& curSentence = m_sentences[word.fontStyle];
		const Rectangle& wordRect = curSentence->MeasureText(update, word.text, startPos);
		lineSpacing = max(lineSpacing, curSentence->GetLineSpacing(update));
		maxHeight = max(maxHeight, wordRect.height);

		//���̿� �Ѿ������ ��������
		if (!usableArea.Contains(wordRect) && usableArea.height < wordRect.y + wordRect.height)
			break;

		if (!usableArea.Contains(wordRect) && usableArea.width < wordRect.x + wordRect.width)	//���̿� �Ѿ�� ���
		{
			startPos.x = 0.0f;
			startPos.y += static_cast<float>(maxHeight) + lineSpacing;
			continue;
		}

		m_lines.emplace_back(move(WordPosition(word, startPos)));
		startPos.x = static_cast<float>(wordRect.x + wordRect.width);
		w++;
	}
	
	return true;
}

void TextArea::Update(const Vector2& resolution) noexcept
{
	m_position = m_layout->GetPosition(resolution);
}

void TextArea::Render(IRender* render)
{
	for (const auto& word : m_lines)
	{
		const auto& curSentence = m_sentences[word.fontStyle];
		curSentence->DrawString(render, m_position, word);
	}
}