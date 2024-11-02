#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;
class Panel;

class Dialog : public IRenderItem
{
public:
	Dialog();
	~Dialog();

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;

	bool SetUIItem();

private:
	vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
	unique_ptr<Panel> m_panel;
};