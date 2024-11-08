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
	Dialog(const Dialog& other) noexcept;
	~Dialog();

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;
	virtual IRenderItem* GetSelected() const noexcept override;
	virtual unique_ptr<IRenderItem> Clone() override;
	virtual void SetPosition(const string& name, const Vector2& position) noexcept override;
	virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }

	void SetName(const string& name);
	void AddRenderItem(const Vector2& normalPos, unique_ptr<IRenderItem>&& renderItem);

private:
	string m_name{};
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<RenderItemProperty>> m_renderItems;
};