#include "pch.h"
#include "TextArea.h"
#include "../../Include/IRenderer.h"
#include "UILayout.h"
#include "../Utility.h"

TextArea::TextArea() :
	m_layout{ nullptr }
{}

bool TextArea::LoadResources(ILoadData* load)
{
	size_t index{ 0 };
	for (const auto& filename : m_fontFilenames)
		ReturnIfFalse(load->LoadFont(filename, index));

	return true;
}

void TextArea::Render(IRender* render)
{
	render;
}

void TextArea::SetFont(const wstring& resPath, std::vector<wstring> fontFilenames, const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);
	ranges::transform(fontFilenames, back_inserter(m_fontFilenames), [&resPath](const auto& filename) {
		return resPath + filename;
		});
}

