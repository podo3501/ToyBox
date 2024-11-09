#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual bool SetResources(const wstring& filename) = 0;
	virtual bool LoadResources(ILoadData* load) = 0;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* mouseTracker) noexcept = 0;
	virtual void Render(IRender* render) = 0;
	virtual bool IsPicking(const Vector2& pos)  const noexcept = 0;
	virtual const Rectangle& GetArea() const noexcept = 0;
	virtual void SetSelected(const string& name, bool selected) noexcept = 0;
	virtual IRenderItem* GetSelected() const noexcept = 0;
	virtual unique_ptr<IRenderItem> Clone() = 0;
	virtual void SetPosition(const string& name, const Vector2& pos) noexcept = 0;
	virtual bool ChangeArea(const Rectangle& area) noexcept = 0;
	virtual const string& GetName() const noexcept = 0;
	virtual IRenderItem* GetRenderItem(const string& name) const noexcept = 0;
};