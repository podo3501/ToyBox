#pragma once
#include "../../Include/IRenderItem.h"

struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public IRenderItem
{
public:
	~TextArea();
	TextArea();
	TextArea(const Vector2& position, const UILayout* layout,
		const map<wstring, wstring>& fontFileList,
		const map<wstring, size_t>& font,
		const vector<TextData>& lines);

	virtual bool SetResources(const wstring& filename) override { filename; return true; }
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2&)  const noexcept override { return false; }
	virtual const Rectangle& GetArea() const noexcept override;
	virtual IRenderItem* GetSelected() const noexcept { return nullptr; }
	virtual unique_ptr<IRenderItem> Clone();
	virtual void SetPosition(const string& name, const Vector2&) noexcept override;
	virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }

	void SetFont(IRenderer* renderer, 
		const Vector2& position,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);
	bool SetText(IGetValue* update, wstring&& text);

private:
	Vector2 m_position{};
	unique_ptr<UILayout> m_layout;
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
