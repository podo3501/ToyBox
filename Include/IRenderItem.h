#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual bool LoadResources(ILoadData* load) = 0;
	virtual void Render(IRender* render) = 0;
	virtual bool IsPicking(const Vector2& pos)  const noexcept = 0;
	virtual const Rectangle& GetArea() const noexcept = 0;
};
