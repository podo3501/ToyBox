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
	virtual void ImplementRender(ITextureRender* render) const override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;

private:
	bool ArrangeText(const wstring& text);

	ITextureController* m_texController;
	wstring m_text{};
	map<wstring, wstring> m_fontFileList;
	map<wstring, size_t> m_font; //core���¿� ���� �ε����� ���Ҽ� �ֱ� ������ �������� �ʴ´�.
	vector<TextData> m_lines;
};

unique_ptr<TextArea> CreateTextArea(const UILayout& layout, 
	const wstring& text, map<wstring, wstring>& fontFileList);