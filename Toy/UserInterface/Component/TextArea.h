#pragma once
#include "../UIComponent.h"

struct IGetValue;
struct IRenderer;
class UILayout;
struct TextData;

class TextArea : public UIComponent
{
public:
	~TextArea();
	TextArea();

	static ComponentID GetTypeStatic() { return ComponentID::TextArea; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual void SerializeIO(JsonOperation& operation) override;

	void SetFont(const wstring& text, const UILayout& layout, const map<wstring, wstring>& fontFileList) noexcept;

protected:
	TextArea(const TextArea& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementUpdatePosition(const XMINT2& position) noexcept override;
	virtual void ImplementRender(IRender* render) const override;

private:
	Vector2 m_posByResolution{};
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font;
	vector<TextData> m_lines;
};

unique_ptr<TextArea> CreateTextArea(const UILayout& layout, 
	const wstring& text, map<wstring, wstring>& fontFileList);