#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;

class RenderItemProperty;
class UILayout;
class Panel;

class Dialog : public IRenderItem
{
public:
	Dialog();
	~Dialog();

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;

	IRenderItem* GetSelected() const noexcept;
	//void Update() noexcept;
	bool SetUIItem();

private:
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<RenderItemProperty>> m_renderItems;
	unique_ptr<Panel> m_panel;
};