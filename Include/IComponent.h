#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class MouseTracker;

struct IComponent
{
public:
	virtual ~IComponent() {};

	virtual bool LoadResources(ILoadData* load) = 0;
	virtual bool SetDatas(IGetValue*) = 0;
	virtual bool Update(const Vector2& position, MouseTracker* mouseTracker) noexcept = 0;
	virtual void Render(IRender* render) = 0;
};
