#pragma once
#include "IRenderItem.h"

struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public IRenderItem
{
public:
	~TextArea();
	TextArea();
	TextArea(const TextArea& other);

	virtual unique_ptr<IRenderItem> Clone();
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* render) override;

	void SetFont(const string& name,
		const Vector2& position,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);
	bool SetText(IGetValue* update, wstring&& text);
	void SetPosition(const Vector2&) noexcept;

private:
	Vector2 m_position{};
	Vector2 m_posByResolution{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
