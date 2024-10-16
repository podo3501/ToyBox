#include "pch.h"
#include "TextArea.h"
#include "../../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"
#include "UIUtility.h"

class Sentence
{
public:
	Sentence() = default;
	~Sentence() = default;

	bool Load(ILoadData* load, const wstring& filename)
	{
		return load->LoadFont(filename, m_index);
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

bool TextArea::SetText(IUpdate* update, wstring&& text) noexcept
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(text, textProperty));
	//if (m_sentences.find(fontName) == m_sentences.end()) return false;
	//ũ�⸦ ���ͼ� ���� ������ ���� �� ������ ���Ѵ�.
	//m_sentences[fontName]->GetSize(text);
	//m_text = move(text);
	return true;
}

void TextArea::Update(const Vector2& resolution) noexcept
{
	m_position = m_layout->GetPosition(resolution);
}

void TextArea::Render(IRender* render)
{
	render;
}