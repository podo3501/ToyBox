#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;
class Panel;

class Dialog : public IRenderItem
{
public:
	Dialog() = delete;
	Dialog(IRenderer* renderer);
	~Dialog();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;

	bool SetUIItem();

private:
	IRenderer* m_renderer;
	unique_ptr<Panel> m_panel;
};