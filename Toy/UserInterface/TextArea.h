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

	virtual bool SetResources(const wstring& filename) override { filename; return true; }
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2&)  const noexcept override { return false; }
	virtual const Rectangle& GetArea() const noexcept override;
	virtual void SetSelected(const string& name, bool selected) noexcept {};
	virtual IRenderItem* GetSelected() const noexcept { return nullptr; }
	virtual unique_ptr<IRenderItem> Clone();
	virtual void SetPosition(const string& name, const Vector2&) noexcept override;
	virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }
	virtual const string& GetName() const noexcept { return m_name; }
	virtual IRenderItem* GetRenderItem(const string& name) const noexcept override { return nullptr; }

	void SetFont(const string& name,
		const Vector2& position,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);
	bool SetText(IGetValue* update, wstring&& text);

private:
	string m_name{};
	Vector2 m_position{};
	Vector2 m_posByResolution{};
	unique_ptr<UILayout> m_layout;
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
