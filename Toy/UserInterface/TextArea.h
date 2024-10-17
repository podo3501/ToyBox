#pragma once
#include "../../Include/IRenderItem.h"

class UILayout;
class Sentence;
struct WordPosition;

class TextArea : public IRenderItem
{
public:
	TextArea();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* render) override;

	void Update(const Vector2& resolution) noexcept;
	void SetFont(const wstring& resPath, const map<wstring, wstring>& fontFileList, const UILayout& layout);
	bool SetText(IUpdate* update, wstring&& text);

private:
	unique_ptr<UILayout> m_layout;
	map<wstring, wstring> m_fontFileList;
	map<wstring, unique_ptr<Sentence>> m_sentences;
	vector<WordPosition> m_lines;
	
	XMUINT2 m_position{};
};
