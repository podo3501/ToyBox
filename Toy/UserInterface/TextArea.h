#pragma once
#include "../../Include/IRenderItem.h"

class UILayout;

class TextArea : public IRenderItem
{
public:
	TextArea();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* render) override;

	void SetFont(const wstring& resPath, std::vector<wstring> fontFilenames, const UILayout& layout);

private:
	unique_ptr<UILayout> m_layout;
	std::vector<wstring> m_fontFilenames;
};
