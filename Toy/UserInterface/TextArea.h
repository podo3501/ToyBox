#pragma once
#include "UIComponent.h"

struct IGetValue;
struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public UIComponent
{
protected:
	TextArea(const TextArea& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~TextArea();
	TextArea();

	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept override;
	virtual void Render(IRender* render) override;

	void SetFont(const string& name,
		const wstring& text,
		const UILayout& layout,
		const map<wstring, wstring>& fontFileList);

	virtual void SerializeIO(JsonOperation& operation) override;

private:
	Vector2 m_posByResolution{};
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};
