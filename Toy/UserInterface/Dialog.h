#pragma once
#include "../Include/IRenderItem.h"

class UILayout;
struct ImageSource;
enum class Origin;

class Dialog : public IRenderItem
{
public:
	Dialog(const wstring& resPath);
	virtual ~Dialog();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;

	void SetImage(const ImageSource& sources, const Rectangle& area, const XMFLOAT2& pos, Origin origin);
	void Update(const Vector2& resolution);

private:
	wstring m_resPath{};
	unique_ptr<UILayout> m_layout;
};
