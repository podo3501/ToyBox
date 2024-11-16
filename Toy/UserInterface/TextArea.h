#pragma once
#include "UIComponent.h"

struct IGetValue;
struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public UIComponent
{
public:
	~TextArea();
	TextArea();
	TextArea(const TextArea& other);
	TextArea& operator=(const TextArea& o);
	bool operator==(const TextArea& o) const noexcept;

	virtual unique_ptr<UIComponent> Clone();
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* render) override;

	void SetFont(const string& name,
		const wstring& text,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);

	friend void to_json(nlohmann::ordered_json& j, const TextArea& data);
	friend void from_json(const nlohmann::json& j, TextArea& data);

private:
	Vector2 m_posByResolution{};
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
