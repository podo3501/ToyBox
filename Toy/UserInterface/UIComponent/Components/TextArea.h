#pragma once
#include "../UIComponent.h"

struct ITextureController;
struct IRenderer;
struct TextData;
namespace DX { class StepTimer; }

class TextArea : public UIComponent
{
public:
	~TextArea();
	TextArea();

	static ComponentID GetTypeStatic() { return ComponentID::TextArea; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetText(const wstring& text);
	inline const wstring& GetText() const noexcept { return m_text; }
	bool Setup(const UILayout& layout, const wstring& text, const vector<wstring> bindKeys) noexcept;

protected:
	TextArea(const TextArea& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder* resBinder, ITextureController*) noexcept override;
	virtual void ImplementRender(ITextureRender* render) const override;
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept;

private:
	bool ArrangeText(const wstring& text);

	vector<wstring> m_bindKeys;
	wstring m_text{};

	ITextureController* m_texController;
	map<wstring, size_t> m_font; //core상태에 따라서 인덱스는 변할수 있기 때문에 저장하지 않는다.
	vector<TextData> m_lines;
};

unique_ptr<TextArea> CreateTextArea(const UILayout& layout, const wstring& text, const vector<wstring>& bindKeys);