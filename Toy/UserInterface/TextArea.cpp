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

//한글폰트와 영문폰트는 각각 한개만 로딩하기로 한다.
//중간에 볼드나 밑줄같은 것은 지원하지 않고 크기도 고정으로 한다.
//나중에 고려하도록 한다.
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
	//크기를 얻어와서 현재 공간에 넣을 수 있을지 비교한다.
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