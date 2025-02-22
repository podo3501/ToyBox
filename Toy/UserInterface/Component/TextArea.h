#pragma once
#include "../UIComponent.h"

struct ITextureController;
struct IRenderer;
class UILayout;
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
	void SetFont(const wstring& text, const UILayout& layout, const map<wstring, wstring>& fontFileList) noexcept;

protected:
	TextArea(const TextArea& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementLoadResource(ITextureLoad* load) override;
	virtual bool ImplementPostLoaded(ITextureController* texController) override;
	virtual bool ImplementUpdatePosition(const DX::StepTimer&, const XMINT2& position) noexcept override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	bool ArrangeText(const wstring& text);

	ITextureController* m_texController;
	Vector2 m_posByResolution{};
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font; //core상태에 따라서 인덱스는 변할수 있기 때문에 저장하지 않는다.
	vector<TextData> m_lines;
};

unique_ptr<TextArea> CreateTextArea(const UILayout& layout, 
	const wstring& text, map<wstring, wstring>& fontFileList);