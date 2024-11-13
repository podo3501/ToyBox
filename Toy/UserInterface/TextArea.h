#pragma once
#include "UIComponent.h"

struct IGetValue;
struct IRenderer;
class UILayout;
struct TextData;
class JsonOperation;

class TextArea : public UIComponent
{
public:
	~TextArea();
	TextArea();
	TextArea(const TextArea& other);

	virtual unique_ptr<UIComponent> Clone();
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* render) override;

	bool IsEqual(const TextArea* other) const noexcept;
	void SetFont(const string& name,
		const wstring& text,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);
	bool Write(const wstring& filename) const noexcept;
	bool Read(const wstring& filename) noexcept;
	void FileIO(JsonOperation* operation);

private:
	void ToJson(nlohmann::ordered_json& outJson) const noexcept;
	bool FromJson(const nlohmann::json& j) noexcept;

	Vector2 m_posByResolution{};
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
