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
	virtual bool Update(const Vector2& position) noexcept = 0;
	virtual void Render(IRender* render) = 0;
	virtual bool IsPicking(const Vector2& pos)  const noexcept = 0;
	virtual const Rectangle& GetArea() const noexcept = 0;
	virtual IRenderItem* GetSelected() const noexcept = 0;
	virtual unique_ptr<IRenderItem> Clone() = 0;
	virtual void SetPosition(const Vector2& pos) noexcept = 0;
};

struct IRenderScene
{
public:
	virtual ~IRenderScene() {};
	virtual bool LoadScene(ILoadData* load) = 0;
	virtual void RenderScene(IRender* render) = 0;
};
