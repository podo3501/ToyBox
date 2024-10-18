#pragma once
#include "../../Include/IRenderItem.h"

class UILayout;
struct TextData;

class TextArea : public IRenderItem
{
public:
	TextArea();
	~TextArea();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* render) override;

	void Update(const Vector2& resolution) noexcept;
	void SetFont(const wstring& resPath, const map<wstring, wstring>& fontFileList, const UILayout& layout);
	bool SetText(IUpdate* update, wstring&& text);

private:
	unique_ptr<UILayout> m_layout;
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
	
	Vector2 m_position{};
};
