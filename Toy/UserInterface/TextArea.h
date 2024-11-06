#pragma once
#include "../../Include/IRenderItem.h"

struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public IRenderItem
{
public:
	TextArea();
	~TextArea();

	virtual bool SetResources(const wstring& filename) override { filename; return true; }
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2&)  const noexcept override { return false; }
	virtual const Rectangle& GetArea() const noexcept override;

	void SetFont(IRenderer* renderer, const map<wstring, wstring>& fontFileList, const UILayout& layout);
	bool SetText(IGetValue* update, wstring&& text);

private:
	unique_ptr<UILayout> m_layout;
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
	
	Vector2 m_position{};
};
