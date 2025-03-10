#include "pch.h"
#include "TextArea.h"
#include "../Include/IRenderer.h"
#include "../../Utility.h"
#include "../../Config.h"
#include "../UIUtility.h"
#include "../JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

//�ѱ���Ʈ�� ������Ʈ�� ���� �Ѱ��� �ε��ϱ�� �Ѵ�.
//�߰��� ���峪 ���ٰ��� ���� �������� �ʰ� ũ�⵵ �������� �Ѵ�.
TextArea::~TextArea()
{
	Release();
}

void TextArea::Release() noexcept
{
	if (!m_texController) return;

	for (const auto& font : m_font)
		m_texController->ReleaseTexture(font.second);
	m_texController = nullptr;
	m_font.clear();
}

TextArea::TextArea() :
	m_texController{ nullptr }
{}

TextArea::TextArea(const TextArea& other) :
	UIComponent{ other },
	m_texController{ other.m_texController }
{
	m_text = other.m_text;
	m_fontFileList = other.m_fontFileList;
	m_font = other.m_font;
	m_lines = other.m_lines;
}

void TextArea::AddRef() const noexcept
{
	if (!m_texController) return;

	for (auto index : m_font | views::values)
		m_texController->AddRef(index);
}

unique_ptr<UIComponent> TextArea::CreateClone() const
{
	auto clone = unique_ptr<TextArea>(new TextArea(*this));
	clone->AddRef();
	return clone;
}

bool TextArea::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const TextArea* rhs = static_cast<const TextArea*>(&o);
	
	return tie(m_fontFileList, m_text) == tie(rhs->m_fontFileList, rhs->m_text);
}

bool TextArea::ImplementLoadResource(ITextureLoad* load)
{
	Release();

	size_t index{ 0 };
	for (const auto& file : m_fontFileList)
	{
		ReturnIfFalse(load->LoadFont(GetResourceFullFilename(file.second), index));
		m_font.emplace(file.first, index);
	}

	return true;
}

bool TextArea::ArrangeText(const wstring& text)
{
	TextProperty textProperty;
	ReturnIfFalse(Parser(text, textProperty));

	m_lines.clear();
	Rectangle usableArea = XMUINT2ToRectangle(GetSize());
	Vector2 startPos{};
	float lineSpacing = 0.0f;
	long maxHeight = 0;

	vector<TextData> textList = textProperty.GetTextList();
	for (auto& word : textList)
	{
		const auto& fontIdx = m_font[word.fontStyle];
		const Rectangle& wordRect = m_texController->MeasureText(fontIdx, word.text, startPos);
		lineSpacing = max(lineSpacing, m_texController->GetLineSpacing(fontIdx));
		maxHeight = max(maxHeight, wordRect.height);

		if (word.text == L"br")
		{
			startPos = { 0.0f, startPos.y + lineSpacing };
			continue;
		}

		if (usableArea.height < wordRect.y + wordRect.height)
			break; // ���� ����

		if (usableArea.width < wordRect.x + wordRect.width)
			startPos = { 0.0f, startPos.y + lineSpacing };

		if (usableArea.Contains(wordRect))
		{
			word.position = startPos;
			m_lines.push_back(word);
			startPos.x = static_cast<float>(wordRect.x + wordRect.width);
		}
	}

	return true;
}

bool TextArea::SetText(const wstring& text)
{
	if(!ArrangeText(text)) return false;
		
	m_text = text;

	return true;
}

bool TextArea::ImplementPostLoaded(ITextureController* texController)
{
	m_texController = texController;
	return ArrangeText(m_text);
}

bool TextArea::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(UIComponent::ImplementChangeSize(size));
	return ArrangeText(m_text);
}

bool TextArea::Setup(const wstring& text, const UILayout& layout, const map<wstring, wstring>& fontFileList) noexcept
{
	m_text = text;
	SetLayout(layout);
	ranges::transform(fontFileList, inserter(m_fontFileList, m_fontFileList.end()), [](const auto& filename) {
		return make_pair(filename.first, filename.second);
		});

	return true;
}

void TextArea::ImplementRender(ITextureRender* render) const
{
	const auto& position = XMINT2ToVector2(GetPosition());
	for (const auto& word : m_lines)
		render->DrawString(m_font.at(word.fontStyle), 
			word.text, position + word.position,
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
	return CreateIfSetup(move(textArea), text, layout, fontFileList);
}
