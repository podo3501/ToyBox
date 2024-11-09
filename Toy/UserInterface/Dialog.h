#pragma once
#include "IRenderItem.h"

struct IRenderer;

class RenderItemProperty;
class RenderItemList;
class UILayout;
class Panel;

class Dialog : public IRenderItem
{
public:
	Dialog();
	Dialog(const Dialog& other) noexcept;
	~Dialog();

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;
	virtual void SetSelected(const string& name, bool selected) noexcept override;
	virtual IRenderItem* GetSelected() const noexcept override;
	virtual unique_ptr<IRenderItem> Clone() override;
	virtual void SetPosition(const string& name, const Vector2& position) noexcept override;
	virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }
	virtual const string& GetName() const noexcept { return m_name; }
	virtual IRenderItem* GetRenderItem(const string& name) const noexcept override;

	void SetName(const string& name);
	void AddComponent(unique_ptr<IRenderItem>&& renderItem, const Vector2& normalPos);

private:
	string m_name{};
	unique_ptr<UILayout> m_layout;
	unique_ptr<RenderItemList> m_renderItemList;
};